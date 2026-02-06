"""Oakley MODP groups loaded from JSON definition."""
from __future__ import annotations

from dataclasses import dataclass
import json
import os
from typing import Dict


@dataclass(frozen=True)
class OakleyGroup:
    group_id: int
    name: str
    p: int
    g: int = 2

    @property
    def bits(self) -> int:
        return self.p.bit_length()

    @property
    def size_bytes(self) -> int:
        return (self.bits + 7) // 8

    @property
    def q(self) -> int:
        return (self.p - 1) // 2


def _from_hex(hex_text: str) -> int:
    cleaned = hex_text.replace("\n", "").replace(" ", "")
    return int(cleaned, 16)


_GROUPS_PATH = os.path.join(os.path.dirname(__file__), "oakley_groups.json")


def _load_groups() -> Dict[int, OakleyGroup]:
    with open(_GROUPS_PATH, "r", encoding="utf-8") as fh:
        raw = json.load(fh)
    groups: Dict[int, OakleyGroup] = {}
    for key, data in raw.items():
        group_id = int(data.get("group_id") or key.lstrip("group"))
        name = data["name"]
        p_hex = data["p_hex"]
        g_value = int(data.get("g", 2))
        groups[group_id] = OakleyGroup(
            group_id=group_id,
            name=name,
            p=_from_hex(p_hex),
            g=g_value,
        )
    return groups


_GROUPS = _load_groups()


def list_groups() -> Dict[int, OakleyGroup]:
    return dict(_GROUPS)


def get_group(group_id: int) -> OakleyGroup:
    if group_id not in _GROUPS:
        raise KeyError(f"Unknown Oakley group id: {group_id}")
    return _GROUPS[group_id]

