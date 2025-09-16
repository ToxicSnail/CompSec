#!/usr/bin/env python3
import random
from typing import List, Tuple

S_BOX: List[int]     = [3, 7, 2, 5, 1, 6, 0, 4]
S_BOX_INV: List[int] = [S_BOX.index(i) for i in range(8)]

DELTA_LIST_DEC: List[int] = [
    0b000_000_110,  # ΔA₁
    0b000_000_010   # ΔA₂
]

PAIRS_PER_DELTA: int = 5
# KEY_DEC: int | None = None  # None → случайный 9-битный ключ
KEY_DEC = 0b101011001

# ----------------- Утилиты -----------------
def int_to_bits(n: int, width: int) -> str:
    return format(n, f"0{width}b")

def split_to_words(state: int) -> Tuple[int,int,int]:
    """9-битное слово → три 3-битовых слова."""
    return (state >> 6) & 7, (state >> 3) & 7, state & 7

def join_words(w0: int, w1: int, w2: int) -> int:
    """Три 3-битовых слова → одно 9-битное слово."""
    return (w0 << 6) | (w1 << 3) | w2

# ----------------- S-слой / P-слой -----------------
def s_layer(state: int) -> int:
    a0, a1, a2 = split_to_words(state)
    return join_words(S_BOX[a0], S_BOX[a1], S_BOX[a2])

def p_layer(state: int) -> int:
    b = [(state >> i) & 1 for i in range(9)]
    new0 = (b[8]<<2) | (b[5]<<1) | b[2]
    new1 = (b[7]<<2) | (b[4]<<1) | b[1]
    new2 = (b[6]<<2) | (b[3]<<1) | b[0]
    return join_words(new0, new1, new2)

# ----------------- SPN-шифрование -----------------
def encrypt(plain: int, key: int) -> int:
    state = plain ^ key
    state = s_layer(state)
    state = p_layer(state)

    state ^= key
    state = s_layer(state)
    state = p_layer(state)

    state ^= key
    state = s_layer(state)

    return state ^ key  

# ----------------- DDT -----------------
def build_ddt() -> List[List[int]]:
    ddt = [[0]*8 for _ in range(8)]
    for x in range(8):
        for dx in range(8):
            dy = S_BOX[x] ^ S_BOX[x ^ dx]
            ddt[dx][dy] += 1
    return ddt

def print_ddt(ddt: List[List[int]]) -> None:
    header = "ΔA_in \\ ΔC_out | " + " ".join(int_to_bits(i,3) for i in range(8))
    print("\nТаблица распределения дифференциалов (DDT)")
    print(header)
    print("-" * len(header))
    for dx, row in enumerate(ddt):
        print(f"  {int_to_bits(dx,3)}           | " + " ".join(f"{v:3d}" for v in row))
    print()

# ----------------- Генерация пар -----------------
def gen_pairs(delta: int, key: int, n: int) -> List[Tuple[int,int,int,int]]:
    pairs: List[Tuple[int,int,int,int]] = []
    while len(pairs) < n:
        x       = random.randint(0, 0x1FF)
        x_prime = x ^ delta
        y       = encrypt(x, key)
        y_prime = encrypt(x_prime, key)
        pairs.append((x, y, x_prime, y_prime))
    return pairs

def print_pair_table(pairs: List[Tuple[int,int,int,int]], caption: str) -> None:
    print(caption)
    header = "№ | X         | Y         | X′        | Y′        | ΔC"
    print(header)
    print("-" * len(header))
    for i, (x, y, xp, yp) in enumerate(pairs, 1):
        dc = y ^ yp
        print(f"{i:>2} | "
              f"{int_to_bits(x,9)} | "
              f"{int_to_bits(y,9)} | "
              f"{int_to_bits(xp,9)} | "
              f"{int_to_bits(yp,9)} | "
              f"{int_to_bits(dc,9)}")
    print()

# ----------------- main -----------------
def main() -> None:
    random.seed(42)
    key = KEY_DEC if KEY_DEC is not None else random.getrandbits(9)
    print(f"Секретный ключ K = {int_to_bits(key,9)} (dec {key})")

    # Проверка распространения разности через два раунда
    for delta in DELTA_LIST_DEC:
        d1   = s_layer(delta)
        d1p  = p_layer(d1)
        d2   = s_layer(d1p)
        d2p  = p_layer(d2)
        print(f"ΔA={int_to_bits(delta,9)} → вход последнего S: {int_to_bits(d2p,9)}")

    ddt = build_ddt()
    print_ddt(ddt)

    for idx, delta in enumerate(DELTA_LIST_DEC, 1):
        print(f"\n=== Анализ ΔA{idx} = {int_to_bits(delta,9)} ===")
        pairs = gen_pairs(delta, key, PAIRS_PER_DELTA)
        print_pair_table(pairs, f"Таблица {idx}")

if __name__ == "__main__":
    main()
