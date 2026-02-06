"""Helpers for demonstrating Oakley Main Mode with a pre-shared key."""
from __future__ import annotations

import hashlib
import hmac
import json
import os
import secrets
import socket
from dataclasses import dataclass
from typing import Any, Dict, Tuple

import sys
sys.path.insert(0, os.path.dirname(__file__))

from oakley_groups import OakleyGroup, get_group

_CANONICAL_SEPARATORS = (',', ':')
_META_FIELDS = {"name", "display", "description", "meaning"}
_ID_TYPES = {
    "ID_IPV4_ADDR": 1,
    "ID_FQDN": 2,
    "ID_USER_FQDN": 3,
    "ID_IPV4_ADDR_SUBNET": 4,
    "ID_IPV6_ADDR": 5,
    "ID_IPV6_ADDR_SUBNET": 6,
}


@dataclass
class DerivedKeys:
    skeyid: bytes
    skeyid_d: bytes
    skeyid_a: bytes
    skeyid_e: bytes

    def as_hex(self) -> Dict[str, str]:
        return {
            "SKEYID": self.skeyid.hex(),
            "SKEYID_d": self.skeyid_d.hex(),
            "SKEYID_a": self.skeyid_a.hex(),
            "SKEYID_e": self.skeyid_e.hex(),
        }


def _canonical_json(data: Any) -> bytes:
    return json.dumps(data, sort_keys=True, separators=_CANONICAL_SEPARATORS).encode('utf-8')


def _strip_meta_fields(obj: Any) -> Any:
    if isinstance(obj, dict):
        return {k: _strip_meta_fields(v) for k, v in obj.items() if k not in _META_FIELDS}
    if isinstance(obj, list):
        return [_strip_meta_fields(v) for v in obj]
    return obj


def int_to_hex(value: int) -> str:
    return f"0x{value:x}"


def hex_to_int(value: str) -> int:
    text = value.lower().strip()
    if text.startswith("0x"):
        text = text[2:]
    return int(text, 16)


def bytes_to_hex(data: bytes) -> str:
    return data.hex()


def hex_to_bytes(value: str) -> bytes:
    text = value.lower().replace(' ', '')
    if text.startswith("0x"):
        text = text[2:]
    if len(text) % 2 == 1:
        text = '0' + text
    return bytes.fromhex(text)


def int_to_bytes(value: int, length: int) -> bytes:
    return value.to_bytes(length, 'big')


def random_cookie() -> bytes:
    return secrets.token_bytes(8)


def random_nonce(length: int = 16) -> bytes:
    return secrets.token_bytes(length)


def prf(key: bytes, data: bytes) -> bytes:
    return hmac.new(key, data, hashlib.sha1).digest()


def build_default_sa(group: OakleyGroup) -> Dict[str, Any]:
    return {
        "doi": 1,
        "doi_name": "IPsec DOI",
        "situation": ["ID_ONLY"],
        "proposal": {
            "proposal_num": 1,
            "protocol_id": 1,
            "protocol_name": "ISAKMP",
            "spi": "0000000000000000",
            "transforms": [
                {
                    "transform_num": 1,
                    "transform_id": 1,
                    "transform_name": "KEY_IKE",
                    "attributes": [
                        {"type": "ENCRYPTION_ALGORITHM", "value": 5, "meaning": "3DES-CBC"},
                        {"type": "HASH_ALGORITHM", "value": 2, "meaning": "SHA-1"},
                        {"type": "AUTHENTICATION_METHOD", "value": 1, "meaning": "Pre-Shared Key"},
                        {"type": "GROUP_DESCRIPTION", "value": group.group_id, "meaning": group.name},
                        {"type": "LIFE_TYPE", "value": 1, "meaning": "Seconds"},
                        {"type": "LIFE_DURATION", "value": 28800},
                    ],
                }
            ],
        },
    }


def sanitize_sa(sa_payload: Dict[str, Any]) -> Dict[str, Any]:
    return _strip_meta_fields(sa_payload)


def serialize_sa(sa_payload: Dict[str, Any]) -> bytes:
    return _canonical_json(sanitize_sa(sa_payload))


def build_identity_payload(identity: str, identity_type: str = "ID_IPV4_ADDR") -> Dict[str, Any]:
    if identity_type not in _ID_TYPES:
        raise ValueError(f"Unsupported identity type: {identity_type}")
    payload: Dict[str, Any] = {
        "type": identity_type,
        "proto": 0,
        "port": 0,
        "value": identity,
    }
    if identity_type == "ID_IPV4_ADDR":
        socket.inet_aton(identity)  # validate
    return payload


def sanitize_identity(identity_payload: Dict[str, Any]) -> Dict[str, Any]:
    return _strip_meta_fields(identity_payload)


def serialize_identity(identity_payload: Dict[str, Any]) -> bytes:
    return _canonical_json(sanitize_identity(identity_payload))


def derive_key_material(
    psk: bytes,
    ni: bytes,
    nr: bytes,
    shared_secret: bytes,
    cookie_i: bytes,
    cookie_r: bytes,
) -> DerivedKeys:
    skeyid = prf(psk, ni + nr)
    skeyid_d = prf(skeyid, shared_secret + cookie_i + cookie_r + b"\x00")
    skeyid_a = prf(skeyid, skeyid_d + shared_secret + cookie_i + cookie_r + b"\x01")
    skeyid_e = prf(skeyid, skeyid_a + shared_secret + cookie_i + cookie_r + b"\x02")
    return DerivedKeys(skeyid, skeyid_d, skeyid_a, skeyid_e)


def build_header(cookie_i: bytes, cookie_r: bytes, message_id: int = 0, encrypted: bool = False) -> Dict[str, Any]:
    return {
        "cookie_i": bytes_to_hex(cookie_i),
        "cookie_r": bytes_to_hex(cookie_r),
        "message_id": message_id,
        "flags": {
            "encryption": encrypted,
            "commit": False,
        },
    }


class OakleyMainModeInitiator:
    def __init__(
        self,
        psk: bytes,
        identity: str,
        group_id: int = 2,
        nonce_size: int = 16,
    ):
        self.psk = psk
        self.identity = identity
        self.group = get_group(group_id)
        self.nonce_size = nonce_size
        self.cookie_i = random_cookie()
        self.cookie_r = b"\x00" * 8
        self.sa_payload = build_default_sa(self.group)
        self.sa_bytes = serialize_sa(self.sa_payload)
        self.private_key = secrets.randbelow(self.group.q - 2) + 2
        self.public_key = pow(self.group.g, self.private_key, self.group.p)
        self.ni = random_nonce(self.nonce_size)
        self.nr: bytes | None = None
        self.responder_public: int | None = None
        self.shared_secret: bytes | None = None
        self.shared_secret_hex: str | None = None
        self.keys: DerivedKeys | None = None
        self.id_payload = build_identity_payload(self.identity)
        self.id_bytes = serialize_identity(self.id_payload)
        self._size_bytes = self.group.size_bytes
        self.hash_i: bytes | None = None

    def build_message_one(self) -> Dict[str, Any]:
        return {
            "type": "MSG1",
            "header": build_header(self.cookie_i, self.cookie_r),
            "payloads": {
                "SA": self.sa_payload,
                "KE": {
                    "group": self.group.group_id,
                    "public": int_to_hex(self.public_key),
                },
                "Ni": bytes_to_hex(self.ni),
            },
        }

    def process_message_two(self, message: Dict[str, Any]) -> Dict[str, Any]:
        if message.get("type") != "MSG2":
            raise ValueError("Expected MSG2 from responder")
        header = message.get("header", {})
        payloads = message.get("payloads", {})
        cookie_r_hex = header.get("cookie_r")
        if cookie_r_hex is None:
            raise ValueError("Responder missing cookie_r")
        self.cookie_r = hex_to_bytes(cookie_r_hex)
        nr_hex = payloads.get("Nr")
        if nr_hex is None:
            raise ValueError("Responder missing nonce")
        self.nr = hex_to_bytes(nr_hex)
        ke_payload = payloads.get("KE") or {}
        ke_public = ke_payload.get("public")
        if ke_public is None:
            raise ValueError("Responder missing KE payload")
        self.responder_public = hex_to_int(ke_public)
        gxr_bytes = int_to_bytes(self.responder_public, self._size_bytes)
        shared = pow(self.responder_public, self.private_key, self.group.p)
        self.shared_secret = int_to_bytes(shared, self._size_bytes)
        self.shared_secret_hex = self.shared_secret.hex()
        if not self.nr:
            raise ValueError("Responder did not provide nonce")
        self.keys = derive_key_material(self.psk, self.ni, self.nr, self.shared_secret, self.cookie_i, self.cookie_r)
        hash_input = (
            int_to_bytes(self.public_key, self._size_bytes)
            + gxr_bytes
            + self.cookie_i
            + self.cookie_r
            + self.sa_bytes
            + self.id_bytes
        )
        hash_i = prf(self.keys.skeyid, hash_input)
        self.hash_i = hash_i
        msg3 = {
            "type": "MSG3",
            "header": build_header(self.cookie_i, self.cookie_r),
            "payloads": {
                "IDii": self.id_payload,
                "HASH_I": bytes_to_hex(hash_i),
            },
        }
        return msg3

    def process_message_four(self, message: Dict[str, Any]) -> Dict[str, Any]:
        if message.get("type") != "MSG4":
            raise ValueError("Expected MSG4 from responder")
        payloads = message.get("payloads", {})
        idir_payload = payloads.get("IDir")
        if idir_payload is None:
            raise ValueError("Responder did not send IDir")
        idir_bytes = serialize_identity(idir_payload)
        expected = prf(
            self.keys.skeyid,
            int_to_bytes(self.responder_public, self._size_bytes)
            + int_to_bytes(self.public_key, self._size_bytes)
            + self.cookie_r
            + self.cookie_i
            + self.sa_bytes
            + idir_bytes,
        )
        received = hex_to_bytes(payloads.get("HASH_R", ""))
        if expected != received:
            raise ValueError("HASH_R mismatch: responder authentication failed")
        debug = message.get("debug", {})
        self.responder_id = idir_payload
        self.responder_hash = received
        summary = {
            "status": "OK",
            "derived_keys_match": all(
                debug.get(k, "").lower() == v
                for k, v in self.keys.as_hex().items()
            ),
            "responder_id": idir_payload,
        }
        return summary


class OakleyMainModeResponder:
    def __init__(
        self,
        psk: bytes,
        identity: str,
        group_id: int = 2,
        nonce_size: int = 16,
    ):
        self.psk = psk
        self.identity = identity
        self.group = get_group(group_id)
        self.nonce_size = nonce_size
        self.cookie_i: bytes | None = None
        self.cookie_r = random_cookie()
        self.private_key = secrets.randbelow(self.group.q - 2) + 2
        self.public_key = pow(self.group.g, self.private_key, self.group.p)
        self.nr = random_nonce(self.nonce_size)
        self.ni: bytes | None = None
        self.initiator_public: int | None = None
        self.sa_payload: Dict[str, Any] | None = None
        self.sa_bytes: bytes | None = None
        self.shared_secret: bytes | None = None
        self.keys: DerivedKeys | None = None
        self.id_payload = build_identity_payload(self.identity)
        self.id_bytes = serialize_identity(self.id_payload)
        self._size_bytes = self.group.size_bytes
        self.expected_hash_i: bytes | None = None

    def process_message_one(self, message: Dict[str, Any]) -> Dict[str, Any]:
        if message.get("type") != "MSG1":
            raise ValueError("Expected MSG1 from initiator")
        header = message.get("header", {})
        payloads = message.get("payloads", {})
        cookie_i_hex = header.get("cookie_i")
        if cookie_i_hex is None:
            raise ValueError("Initiator did not include cookie")
        self.cookie_i = hex_to_bytes(cookie_i_hex)
        self.sa_payload = payloads.get("SA")
        if self.sa_payload is None:
            raise ValueError("Initiator did not provide SA payload")
        self.sa_bytes = serialize_sa(self.sa_payload)
        ke_payload = payloads.get("KE") or {}
        ke_public = ke_payload.get("public")
        if ke_public is None:
            raise ValueError("Initiator missing KE payload")
        self.initiator_public = hex_to_int(ke_public)
        self.ni = hex_to_bytes(payloads.get("Ni", ""))
        if not self.ni:
            raise ValueError("Initiator missing nonce")
        shared_value = pow(self.initiator_public, self.private_key, self.group.p)
        self.shared_secret = int_to_bytes(shared_value, self._size_bytes)
        self.shared_secret_hex = self.shared_secret.hex()
        msg2 = {
            "type": "MSG2",
            "header": build_header(self.cookie_i, self.cookie_r),
            "payloads": {
                "SA": self.sa_payload,
                "KE": {
                    "group": self.group.group_id,
                    "public": int_to_hex(self.public_key),
                },
                "Nr": bytes_to_hex(self.nr),
            },
        }
        return msg2

    def process_message_three(self, message: Dict[str, Any]) -> Dict[str, Any]:
        if message.get("type") != "MSG3":
            raise ValueError("Expected MSG3 from initiator")
        payloads = message.get("payloads", {})
        idii_payload = payloads.get("IDii")
        if idii_payload is None:
            raise ValueError("Initiator missing IDii payload")
        idii_bytes = serialize_identity(idii_payload)
        provided_hash = hex_to_bytes(payloads.get("HASH_I", ""))
        if not provided_hash:
            raise ValueError("Initiator missing HASH_I payload")
        if self.shared_secret is None or self.ni is None or self.cookie_i is None:
            raise ValueError("Responder state incomplete before MSG3")
        self.keys = derive_key_material(self.psk, self.ni, self.nr, self.shared_secret, self.cookie_i, self.cookie_r)
        expected_hash = prf(
            self.keys.skeyid,
            int_to_bytes(self.initiator_public, self._size_bytes)
            + int_to_bytes(self.public_key, self._size_bytes)
            + self.cookie_i
            + self.cookie_r
            + self.sa_bytes
            + idii_bytes,
        )
        self.expected_hash_i = expected_hash
        if expected_hash != provided_hash:
            raise ValueError("HASH_I mismatch: initiator authentication failed")
        hash_r = prf(
            self.keys.skeyid,
            int_to_bytes(self.public_key, self._size_bytes)
            + int_to_bytes(self.initiator_public, self._size_bytes)
            + self.cookie_r
            + self.cookie_i
            + self.sa_bytes
            + self.id_bytes,
        )
        debug_block = self.keys.as_hex()
        debug_block.update(
            {
                "g_xy": self.shared_secret.hex(),
                "Ni": self.ni.hex(),
                "Nr": self.nr.hex(),
            }
        )
        msg4 = {
            "type": "MSG4",
            "header": build_header(self.cookie_i, self.cookie_r),
            "payloads": {
                "IDir": self.id_payload,
                "HASH_R": bytes_to_hex(hash_r),
            },
            "debug": debug_block,
        }
        return msg4
