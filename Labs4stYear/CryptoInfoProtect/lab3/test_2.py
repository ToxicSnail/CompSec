from __future__ import annotations

import random
import textwrap
from typing import Dict, List, Tuple

S_BOX: List[int] = [3, 7, 2, 5, 1, 6, 0, 4]
S_BOX_INV: List[int] = [S_BOX.index(i) for i in range(8)]
DELTA_LIST_DEC: List[int] = [0b110_000_000, 0b000_000_010]
PAIRS_PER_DELTA: int = 5
KEY_DEC: int | None = None

def int_to_bits(n: int, width: int) -> str:
    return format(n, f"0{width}b")

def split_to_words(state: int) -> Tuple[int, int, int]:
    return (state >> 6) & 7, (state >> 3) & 7, state & 7

def join_words(w0: int, w1: int, w2: int) -> int:
    return (w0 << 6) | (w1 << 3) | w2

def s_layer(state: int) -> int:
    a0, a1, a2 = split_to_words(state)
    return join_words(S_BOX[a0], S_BOX[a1], S_BOX[a2])

def s_layer_inv(state: int) -> int:
    a0, a1, a2 = split_to_words(state)
    return join_words(S_BOX_INV[a0], S_BOX_INV[a1], S_BOX_INV[a2])

def p_layer(state: int) -> int:
    b = [(state >> i) & 1 for i in range(9)]
    new0 = (b[8] << 2) | (b[5] << 1) | b[2]
    new1 = (b[7] << 2) | (b[4] << 1) | b[1]
    new2 = (b[6] << 2) | (b[3] << 1) | b[0]
    return join_words(new0, new1, new2)

def encrypt(plain: int, key: int) -> int:
    state = plain ^ key
    state = s_layer(state)
    state = p_layer(state)
    state ^= key
    state = s_layer(state)
    state = p_layer(state)
    state ^= key
    state = s_layer(state)
    state ^= key
    return state

def build_ddt() -> List[List[int]]:
    ddt = [[0] * 8 for _ in range(8)]
    for x in range(8):
        for dx in range(8):
            dy = S_BOX[x] ^ S_BOX[x ^ dx]
            ddt[dx][dy] += 1
    return ddt

def print_ddt(ddt: List[List[int]]) -> None:
    header = "ΔA_in \\ ΔC_out | " + " ".join(int_to_bits(i, 3) for i in range(8))
    sep = "-" * len(header)
    print("\n", header)
    print(sep)
    for din, row in enumerate(ddt):
        cells = " ".join(f"{v:3d}" for v in row)
        print(f"     {int_to_bits(din,3)}      | {cells}")
    print()

def gen_pairs(delta: int, key: int, n_pairs: int) -> List[Tuple[int, int, int, int]]:
    pairs: List[Tuple[int, int, int, int]] = []
    while len(pairs) < n_pairs:
        x = random.randint(0, 0x1FF)
        x_prime = x ^ delta
        y = encrypt(x, key)
        y_prime = encrypt(x_prime, key)
        pairs.append((x, y, x_prime, y_prime))
    return pairs

def print_pair_table(pairs: List[Tuple[int, int, int, int]], caption: str) -> None:
    print(caption)
    print(f"{'\u2116':<4}{'X':<12}{'Y':<12}{'X⊕ΔA':<12}{'Y′':<12}")
    print("-" * 52)
    for i, (x, y, x_p, y_p) in enumerate(pairs, 1):
        print(f"{i:<4}{int_to_bits(x,9):<12}{int_to_bits(y,9):<12}{int_to_bits(x_p,9):<12}{int_to_bits(y_p,9):<12}")
    print()

def rank_last_subkeys(pairs: List[Tuple[int, int, int, int]],
                      expected_du: Dict[int, int]) -> Dict[str, List[Tuple[int,int]]]:
    results: Dict[str, List[Tuple[int, int]]] = {}
    for block_idx in range(3):
        target = expected_du.get(block_idx)
        if target is None:
            continue
        counter = [0] * 8
        for x, y, x_p, y_p in pairs:
            y_b = (y >> (6 - 3*block_idx)) & 7
            y_pb = (y_p >> (6 - 3*block_idx)) & 7
            for k in range(8):
                u = S_BOX_INV[y_b ^ k]
                u_p = S_BOX_INV[y_pb ^ k]
                if (u ^ u_p) == target:
                    counter[k] += 1
        ranked = sorted([(k, c) for k, c in enumerate(counter)], key=lambda t: -t[1])
        results[f"K{block_idx+1}"] = ranked
    return results

def main() -> None:
    key = KEY_DEC if KEY_DEC is not None else random.getrandbits(9)
    print("K =", int_to_bits(key, 9), "(dec", key, ")\n")
    ddt = build_ddt()
    print_ddt(ddt)
    for idx, delta in enumerate(DELTA_LIST_DEC, 1):
        pairs = gen_pairs(delta, key, PAIRS_PER_DELTA)
        caption = f"\u0422\u0430\u0431\u043b\u0438\u0446\u0430 {idx}: ΔA = {int_to_bits(delta,9)} (dec {delta})"
        print_pair_table(pairs, caption)
        if idx == 1:
            expected = {0: 0b100, 2: 0b100}
            ranked = rank_last_subkeys(pairs, expected)
            print("Candidates:")
            for name, lst in ranked.items():
                best = ", ".join(f"{k:03b}({c})" for k, c in lst[:3])
                print(f"  {name}: {best}")
            print()

if __name__ == "__main__":
    random.seed(42)
    main()