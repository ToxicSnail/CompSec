import random
from itertools import product

SBOX = [3, 7, 2, 5, 1, 6, 0, 4]   # 3-битовый S-блок
SBOX_INV = [SBOX.index(i) for i in range(8)]


# --- Функции для шифрования --------------------------------------
def xor(a, b):
    """Побитовое XOR двух массивов."""
    return [x ^ y for x, y in zip(a, b)]

def substitute(state):
    """Применение S-блока к состоянию."""
    return [SBOX[x] for x in state]

def permute(state):
    """Перестановка бит из книги."""
    s0, s1, s2 = state
    return [
        ((s0 & 0b100) >> 0) | ((s1 & 0b100) >> 1) | ((s2 & 0b100) >> 2),
        ((s0 & 0b010) << 1) | ((s1 & 0b010) >> 0) | ((s2 & 0b010) >> 1),
        ((s0 & 0b001) << 2) | ((s1 & 0b001) << 1) | ((s2 & 0b001) >> 0),
    ]

def round_fn(state, key):
    """Один раунд шифрования."""
    return permute(substitute(xor(state, key)))

def last_round(state, key):
    """Последний раунд без перестановки."""
    return xor(substitute(xor(state, key)), key)

def encrypt(plain, key):
    """Шифрование: два полных раунда + последний."""
    s = round_fn(plain, key)
    s = round_fn(s, key)
    return last_round(s, key)


# --- Таблица распределения дифференциалов --------------------------------
def build_ddt():
    """Построение таблицы распределения дифференциалов для S-блока."""
    table = [[0] * 8 for _ in range(8)]
    for x, dx in product(range(8), repeat=2):
        dy = SBOX[x] ^ SBOX[x ^ dx]
        table[dx][dy] += 1
    return table

def print_ddt(table):
    """Вывод таблицы распределения дифференциалов."""
    header = "ΔA_in \\ ΔC_out | " + " ".join(f'{i:03b}' for i in range(8))
    sep = "-" * len(header)
    print("\nТаблица Распределения Дифференциалов")
    print(header)
    print(sep)
    for din, row in enumerate(table):
        cells = " ".join(f"{v:3}" for v in row)
        print(f"     {din:03b}      | {cells}")
    print()


# --- Таблица пар текстов X, Y, X', Y' --------------------------------
def generate_pair(delta_A):
    """Генерация пары текстов X и X', отличающихся на ΔA."""
    X = [random.randint(0, 1) for _ in range(9)]  # Генерация случайного текста X (9 бит)
    X_prime = [X[i] ^ delta_A[i] for i in range(9)]  # Вычисление X' = X ⊕ ΔA
    return X, X_prime


# --- Основная программа --------------------------------------------
if __name__ == '__main__':
    DDT = build_ddt()
    print_ddt(DDT)

    # Заданная разность ΔA1
    delta_A = [1, 1, 0, 0, 0, 0, 0, 0, 0]  # 110 000 000
    print(f'\n Первое значение выходной разности ΔA1 = {delta_A}')

    # Генерация 5 пар текстов
    pairs = [generate_pair(delta_A) for _ in range(5)]

    # Печать таблицы
    print("\nТаблица результатов шифрования: X, Y, X', Y'\n")
    print(f"{'№':<4}{'X':<10}{'Y':<10}{'X\'':<10}{'Y\'':<10}")  # Заголовок таблицы

    for i, (X, X_prime) in enumerate(pairs, start=1):
        # Генерация случайного ключа (3 бита)
        key = [random.randint(0, 7) for _ in range(3)]
        
        # Шифрование пар X и X'
        Y = encrypt(X, key)
        Y_prime = encrypt(X_prime, key)
        
        # Преобразование Y и Y' в бинарные строки
        Y_bin = ''.join(map(str, Y))
        Y_prime_bin = ''.join(map(str, Y_prime))


      
        
        # Печать результатов (X и X' уже в бинарном виде, Y и Y' теперь тоже)
        print(f"{i:<4}{''.join(map(str, X)):<10}{Y_bin:<10}{''.join(map(str, X_prime)):<10}{Y_prime_bin:<10}")