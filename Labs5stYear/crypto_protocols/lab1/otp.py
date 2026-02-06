import hashlib
from skey_words import SKEY_WORDS


def _md5(data: bytes) -> bytes:
    return hashlib.md5(data).digest()


def _fold64(md5_bytes: bytes) -> bytes:
    """сворачивает 128 бит (16 байт) MD5 в 64 бита (8 байт) XOR’ом левой и правой 64‑битных половин"""
    a = md5_bytes[:8]
    b = md5_bytes[8:16]
    return bytes([a[i] ^ b[i] for i in range(8)])


def _normalize_seed(seed: str) -> str:
    return seed.strip().lower()


def _normalize_passphrase(passphrase: str) -> str:
    return passphrase.encode('ascii', 'ignore').decode('ascii')


def f(value: bytes) -> bytes:
    return _fold64(_md5(value))


def initial_value(seed: str, passphrase: str) -> bytes:
    s = _normalize_seed(seed)
    p = _normalize_passphrase(passphrase)
    return _fold64(_md5((s + p).encode('ascii')))


def iterate(seed: str, passphrase: str, n: int) -> bytes:
    """v_n = f^n(v0)."""
    value = initial_value(seed, passphrase)
    for _ in range(n):
        value = f(value)
    return value


def compute_server_stored(seed: str, passphrase: str, seq: int) -> bytes:
    return iterate(seed, passphrase, seq)


def compute_client_otp(seed: str, passphrase: str, seq: int) -> bytes:
    """что должен прислать клиент при вызове с челленджем “seq seed” — это v_(seq-1)."""
    if seq <= 0:
        raise ValueError("Sequence must be >= 1")
    return iterate(seed, passphrase, seq - 1)


def to_hex64(value: bytes) -> str:
    return value.hex().upper()


def from_hex64(hex_str: str) -> bytes:
    s = hex_str.strip().replace(" ", "").lower()
    if len(s) != 16:
        raise ValueError("Hex must be 16 characters for 64-bit value")
    return bytes.fromhex(s)


def _checksum2(value64: int) -> int:
    total = 0
    for i in range(16):
        total += (value64 >> (i * 4)) & 0xF
    return total & 0x3


def to_words(value: bytes) -> str:
    if len(value) != 8:
        raise ValueError("Value must be 8 bytes")
    v = int.from_bytes(value, 'big')
    c = _checksum2(v)
    combined = (v << 2) | c  # 66 бит

    words = []
    for i in range(5, -1, -1):
        idx = (combined >> (i * 11)) & 0x7FF
        words.append(SKEY_WORDS[idx])
    return ' '.join(words)


def from_words(words: str) -> bytes:
    parts = words.strip().upper().split()
    if len(parts) != 6:
        raise ValueError("Must be exactly 6 words")
    combined = 0
    for w in parts:
        try:
            idx = SKEY_WORDS.index(w)
        except ValueError:
            raise ValueError(f"Invalid word: {w}")
        combined = (combined << 11) | idx

    c = combined & 0x3
    v = combined >> 2  # 64 бит
    if _checksum2(v) != c:
        raise ValueError("Checksum mismatch in words")
    return v.to_bytes(8, 'big')


def parse_password(input_str: str) -> bytes:
    # Restricted: accept only 6-word S/KEY format
    s = input_str.strip()
    if len(s.split()) == 6:
        return from_words(s)
    raise ValueError("Password must be exactly 6 words")
