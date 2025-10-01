import hashlib
import secrets


def md5_chap_response(identifier: int, secret: str, challenge: bytes) -> bytes:
    """RFC 1994: отклик = MD5(Identifier || Secret || Challenge)."""
    if not (0 <= identifier <= 255):
        raise ValueError("Identifier must be 0..255")
    id_byte = bytes([identifier & 0xFF])
    secret_bytes = secret.encode('utf-8')
    m = hashlib.md5()
    m.update(id_byte)
    m.update(secret_bytes)
    m.update(challenge)
    return m.digest()


def generate_challenge(length: int = 16) -> bytes:
    if length < 4 or length > 255:
        raise ValueError("Challenge length must be in 4..255")
    return secrets.token_bytes(length)


def to_hex(b: bytes) -> str:
    return b.hex().upper()


def from_hex(s: str) -> bytes:
    h = s.strip().replace(" ", "").replace(":", "").lower()
    if len(h) % 2 != 0:
        raise ValueError("Hex string must have even length")
    return bytes.fromhex(h)


def compute_client_response(identifier: int, secret: str, challenge_hex: str) -> str:
    ch = from_hex(challenge_hex)
    resp = md5_chap_response(identifier, secret, ch)
    return to_hex(resp)
