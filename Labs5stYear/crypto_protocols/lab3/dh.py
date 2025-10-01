"""Diffie-Hellman core primitives for lab3 (RFC 2631 inspired)."""
from __future__ import annotations

import hashlib
import math
import secrets
from dataclasses import dataclass
from typing import Callable, Optional


RandomBits = Callable[[int], int]


@dataclass
class DomainParameters:
    """Domain parameters (p, g, q) as described in RFC 2631."""

    p: int
    g: int
    q: int
    name: str = "custom"

    def size_bits(self) -> int:
        return self.p.bit_length()

    def as_dict(self) -> dict[str, str]:
        return {
            "name": self.name,
            "p": str(self.p),
            "g": str(self.g),
            "q": str(self.q),
            "bits": str(self.size_bits()),
        }


def _decompose(n: int) -> tuple[int, int]:
    """Decompose n-1 as 2^s * d (Miller-Rabin helper)."""
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    return s, d


def is_probable_prime(n: int, rounds: int = 64) -> bool:
    """Miller-Rabin primality test; suitable for hundreds of bits."""
    if n < 2:
        return False
    small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
    for p in small_primes:
        if n == p:
            return True
        if n % p == 0:
            return False
    s, d = _decompose(n)
    for _ in range(rounds):
        a = secrets.randbelow(n - 3) + 2
        x = pow(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def random_odd_candidate(bits: int, randfunc: Optional[RandomBits] = None) -> int:
    if bits < 2:
        raise ValueError("bits must be >= 2")
    rand = randfunc or secrets.randbits
    candidate = rand(bits)
    candidate |= (1 << (bits - 1))  # ensure high bit set
    candidate |= 1  # ensure odd
    return candidate


def generate_prime(bits: int, rounds: int = 64, randfunc: Optional[RandomBits] = None) -> int:
    """Generate probable prime with Miller-Rabin."""
    if bits < 2:
        raise ValueError("bits must be >= 2")
    while True:
        candidate = random_odd_candidate(bits, randfunc)
        if is_probable_prime(candidate, rounds=rounds):
            return candidate


def generate_safe_prime(bits: int, rounds: int = 64, randfunc: Optional[RandomBits] = None) -> tuple[int, int]:
    """Generate safe prime p = 2q + 1 where q is also prime."""
    if bits < 256:
        raise ValueError("Safe prime should be at least 256 bits for this lab")
    rand = randfunc or secrets.randbits
    while True:
        q = generate_prime(bits - 1, rounds=rounds, randfunc=rand)
        p = 2 * q + 1
        if is_probable_prime(p, rounds=rounds):
            return p, q


def find_generator(p: int, q: int, randfunc: Optional[Callable[[int], int]] = None) -> int:
    """Find generator of subgroup of order q modulo p (safe prime)."""
    if p <= 2 or q <= 2:
        raise ValueError("Invalid domain parameters")
    rand = randfunc or secrets.randbelow
    exponent = (p - 1) // q
    while True:
        h = rand(p - 3) + 2  # 2 .. p-2
        g = pow(h, exponent, p)
        if g in (1, p - 1):
            continue
        if pow(g, q, p) != 1:
            continue
        return g


def make_parameters(bits: int, rounds: int = 64) -> DomainParameters:
    """Generate full set of parameters (p, g, q)."""
    p, q = generate_safe_prime(bits, rounds=rounds)
    g = find_generator(p, q)
    return DomainParameters(p=p, g=g, q=q, name=f"safe-prime-{bits}")


def validate_parameters(params: DomainParameters) -> bool:
    """Basic checks in line with RFC 2631 section 2.1."""
    p, g, q = params.p, params.g, params.q
    if p <= 0 or g <= 0 or q <= 0:
        return False
    if (p - 1) % q != 0:
        return False
    if pow(g, q, p) != 1:
        return False
    if g <= 1 or g >= p - 1:
        return False
    return True


def generate_private_key(q: int) -> int:
    """Generate private exponent 1 < x < q as per RFC guidance."""
    if q <= 3:
        raise ValueError("q too small")
    while True:
        x = secrets.randbelow(q - 2) + 2  # range [2, q-1]
        if 1 < x < q:
            return x


def compute_public_key(g: int, x: int, p: int) -> int:
    return pow(g, x, p)


def validate_public_key(y: int, params: DomainParameters) -> bool:
    """Validate other party public key (RFC 2631 section 2.1.5)."""
    if y <= 1 or y >= params.p - 1:
        return False
    if pow(y, params.q, params.p) != 1:
        return False
    return True


def derive_shared_secret(peer_public: int, private_key: int, params: DomainParameters) -> int:
    if not validate_public_key(peer_public, params):
        raise ValueError("Invalid peer public key")
    return pow(peer_public, private_key, params.p)


def compute_kdf_sha256(shared_secret: int) -> str:
    """Simple SHA-256 KDF (for verification/printing)."""
    zz_bytes = int_to_bytes(shared_secret)
    return hashlib.sha256(zz_bytes).hexdigest()


def int_to_bytes(value: int) -> bytes:
    if value < 0:
        raise ValueError("value must be non-negative")
    if value == 0:
        return b"\x00"
    length = (value.bit_length() + 7) // 8
    return value.to_bytes(length, byteorder="big")


def int_to_hex(value: int) -> str:
    return int_to_bytes(value).hex().upper()


def parse_int(value: str) -> int:
    s = value.strip().lower()
    if s.startswith("0x"):
        return int(s, 16)
    return int(s, 10)


