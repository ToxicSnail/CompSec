"""Selected MODP groups from RFC 3526 for lab3."""
from __future__ import annotations

from dataclasses import dataclass
import json
import os
import sys
from typing import Dict

sys.path.insert(0, os.path.dirname(__file__))

from dh import DomainParameters


@dataclass(frozen=True)
class Rfc3526Group:
    name: str
    bits: int
    p_hex: str
    g: int = 2

    def to_domain_parameters(self) -> DomainParameters:
        hex_clean = self.p_hex.replace("\n", "").replace(" ", "")
        p = int(hex_clean, 16)
        q = (p - 1) // 2
        return DomainParameters(p=p, g=self.g, q=q, name=self.name)


_GROUPS_PATH = os.path.join(os.path.dirname(__file__), "rfc3526_groups.json")


def _load_groups() -> Dict[str, Rfc3526Group]:
    with open(_GROUPS_PATH, "r", encoding="utf-8") as fh:
        raw = json.load(fh)
    groups: Dict[str, Rfc3526Group] = {}
    for key, data in raw.items():
        groups[key.lower()] = Rfc3526Group(
            name=data["name"],
            bits=int(data["bits"]),
            p_hex=str(data["p_hex"]),
            g=int(data.get("g", 2)),
        )
    return groups


_GROUPS = _load_groups()


def list_groups() -> list[str]:
    return sorted(_GROUPS.keys())


def get_group(name: str) -> DomainParameters:
    key = name.strip().lower()
    if key not in _GROUPS:
        raise KeyError(f"Unknown RFC3526 group: {name}")
    return _GROUPS[key].to_domain_parameters()
