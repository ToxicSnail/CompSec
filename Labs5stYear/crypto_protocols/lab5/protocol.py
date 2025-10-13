import hashlib
import json
import os
import random
from dataclasses import dataclass, asdict
from typing import Any, Dict, Optional, Tuple

def canonical_json(obj: Dict[str, Any]) -> bytes:
    return json.dumps(obj, sort_keys=True, separators=(",", ":")).encode("utf-8")


def sha256(data: bytes) -> bytes:
    return hashlib.sha256(data).digest()


def hex_encode(b: bytes) -> str:
    return b.hex()


def hex_decode(s: str) -> bytes:
    s = s.lower().replace(" ", "")
    if s.startswith("0x"):
        s = s[2:]
    if len(s) % 2 == 1:
        s = "0" + s
    return bytes.fromhex(s)


# --- Minimal textbook RSA for signatures (educational) ---

def _egcd(a: int, b: int) -> Tuple[int, int, int]:
    if a == 0:
        return (b, 0, 1)
    g, y, x = _egcd(b % a, a)
    return (g, x - (b // a) * y, y)


def _modinv(a: int, m: int) -> int:
    g, x, _ = _egcd(a, m)
    if g != 1:
        raise ValueError("modular inverse does not exist")
    return x % m


def _is_probable_prime(n: int, k: int = 8) -> bool:
    if n <= 1:
        return False
    small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
    for p in small_primes:
        if n % p == 0:
            return n == p
    # Miller-Rabin
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2
    for _ in range(k):
        a = random.randrange(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def _random_prime(bits: int) -> int:
    while True:
        cand = random.getrandbits(bits) | 1 | (1 << (bits - 1))
        if _is_probable_prime(cand):
            return cand


@dataclass
class RSAPublicKey:
    n: int
    e: int

    def as_dict(self) -> Dict[str, str]:
        return {"n": str(self.n), "e": str(self.e)}


@dataclass
class RSAPrivateKey:
    n: int
    d: int


def rsa_generate(bits: int = 1024, e: int = 65537) -> Tuple[RSAPublicKey, RSAPrivateKey]:
    half = bits // 2
    p = _random_prime(half)
    q = _random_prime(half)
    while p == q:
        q = _random_prime(half)
    n = p * q
    phi = (p - 1) * (q - 1)
    if os.getenv("LAB5_RSA_E"):
        e = int(os.getenv("LAB5_RSA_E"))
    # ensure e is valid
    while True:
        try:
            d = _modinv(e % phi, phi)
            break
        except ValueError:
            e += 2
    return RSAPublicKey(n=n, e=e), RSAPrivateKey(n=n, d=d)


def rsa_sign(priv: RSAPrivateKey, message: bytes) -> int:
    h = int.from_bytes(sha256(message), "big")
    return pow(h, priv.d, priv.n)


def rsa_verify(pub: RSAPublicKey, message: bytes, signature: int) -> bool:
    h = int.from_bytes(sha256(message), "big")
    return pow(signature, pub.e, pub.n) == (h % pub.n)


@dataclass
class Token:
    n: int
    time: int
    requester: str
    doc_hash: str  # hex
    prev_summary: Optional[Dict[str, Any]]  # includes n, requester, doc_hash, time, link_hash
    link_hash: str  # hex of cumulative chain

    def body(self) -> Dict[str, Any]:
        return asdict(self)

    def canonical(self) -> bytes:
        return canonical_json(self.body())


def summarize_token(tok: "Token") -> Dict[str, Any]:
    return {
        "n": tok.n,
        "time": tok.time,
        "requester": tok.requester,
        "doc_hash": tok.doc_hash,
        "link_hash": tok.link_hash,
    }


def compute_link_hash(n: int, requester: str, doc_hash_hex: str, timestamp: int, prev_link_hex: str) -> str:
    data = (
        str(n).encode()
        + b"|" + requester.encode("utf-8")
        + b"|" + hex_decode(doc_hash_hex)
        + b"|" + str(timestamp).encode()
        + b"|" + hex_decode(prev_link_hex)
    )
    return hex_encode(sha256(data))

