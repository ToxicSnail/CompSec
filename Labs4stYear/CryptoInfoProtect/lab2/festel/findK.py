import cipher
import random
import main
from fractions import Fraction
from itertools import product
from typing import List, Tuple, Dict

def generate_plain_text():
    return random.randint(0, 65535)

def generate_selection(max_equation_length, N):
    selection = []
    for _ in range(N):
        pt = generate_plain_text()
        ct = cipher.encrypt(pt, main.KEY)
        # Дополняем нулями до максимальной длины уравнения
        bin_pt = bin(pt)[2:].zfill(max_equation_length)
        bin_ct = bin(ct)[2:].zfill(max_equation_length)
        selection.append([bin_pt, bin_ct])
    return selection

def make_T(selection, equ, N):
    T = []
    count = 0
    for i in range(len(equ)):  # Сначала идём по уравнениям
        for j in range(N):  # Потом подставляем все пары открытых/закрытых текстов
            res_X = 0
            res_Y = 0

            # Проход по коэффициентам X
            for k in range(len(equ[i][0])):  # Обрабатываем все биты уравнения
                if equ[i][0][k] == 1:
                    res_X ^= int(selection[j][0][k])  # Бит из открытого текста

            # Проход по коэффициентам Y
            for k in range(len(equ[i][1])):  # Обрабатываем все биты уравнения
                if equ[i][1][k] == 1:
                    res_Y ^= int(selection[j][1][k])  # Бит из закрытого текста

            # Финальный результат уравнения
            res_left = res_X ^ res_Y
            # print(f"Уравнение {i + 1}, текст {j + 1}: {res_left}")
            if res_left == 0:
                count += 1

        T.append(count)
        count = 0

    return T

def make_left_part(pr, T, N):
    left_part = []
    for i in range(len(T)):
        if T[i] > N / 2:
            if (pr[i] / 16) > (1 / 2): 
                left_part.append(0)
            elif (pr[i] / 16) < (1 / 2):
                left_part.append(1)
        elif T[i] < N / 2:
            if (pr[i] / 16) > (1 / 2):
                left_part.append(1)
            elif (pr[i] / 16) < (1 / 2):
                left_part.append(0)
    return left_part

def make_K(left_part, equ):
    K_equ = []
    for i in range(len(left_part)):
        K_equ.append([left_part[i], equ[i]])

    return K_equ

# === Форматированный вывод анализа уравнений в виде таблицы как в отчёте ===
def print_equation_table(block_num, T_data, pr_values, K_equ, N=100):
    print(f"\n Таблица {block_num}. Анализ Блока {block_num}")
    print(" № | Эффективное линейное уравнение                    = Правая часть         | = |      T     |   p   |   Δ")
    print(" --+--------------------------------------------------------------------------+---+------------+-------+-------")
    for idx, ((t_val, eq), (left_val, _)) in enumerate(zip(T_data, K_equ), 1):
        p_raw = pr_values[idx - 1]
        p_frac = Fraction(p_raw, 16)  # Сокращённая дробь
        delta = abs(1 - 2 * (p_raw / 16))

        # Формируем левую и правую часть уравнения
        x_terms = [f"X{i}" for i, bit in enumerate(eq[0]) if bit == 1]
        y_terms = [f"Y{i}" for i, bit in enumerate(eq[1]) if bit == 1]
        if block_num == 1:
            k_terms = [f"K{i+1}" for i, bit in enumerate(eq[2]) if bit == 1]
        elif block_num == 2:
            k_terms = [f"K{i+5}" for i, bit in enumerate(eq[2]) if bit == 1]
        elif block_num == 3:
            k_terms = [f"K{i+9}" for i, bit in enumerate(eq[2]) if bit == 1]
        else:
            k_terms = []

        lhs = " ⊕ ".join(x_terms + y_terms) if (x_terms + y_terms) else "0"
        rhs = " ⊕ ".join(k_terms) if k_terms else "0"

        # Вывод строки таблицы
        print(f"{idx:2} | {lhs:<50}= {rhs:<20} | {left_val} | {t_val:4}/{N}   | {str(p_frac):>5} | {delta:.3f}")


# === Генерация и печать 10 пар текстов ===
def print_sample_texts(selection, n=10):
    print("\nПримеры 10 пар (Открытый текст — Зашифрованный текст):")
    print("PT                -> CT")
    print("-----------------------------")
    for i in range(min(n, len(selection))):
        pt = int(selection[i][0], 2)
        ct = int(selection[i][1], 2)
        print(f"{pt:016b} -> {ct:016b}")

def extract_key_eqs(K_equ, block_offset: int = 0):
    """
    Преобразует данные из print_equation_table в линейную систему над F2.

    Parameters
    ----------
    K_equ : iterable
        Элементы вида (result, equation), где
            result            – 0 или 1 (правая часть уравнения);
            equation[2]       –   битовый вектор: 1 → бит Ki участвует, 0 → нет.
    block_offset : int, optional
        Сдвиг нумерации ключевых битов относительно глобального (для второго, третьего блока …).

    Returns
    -------
    key_eqs : list[tuple[str, …, int]]
        Каждый кортеж – имена участвующих Ki в произвольном числе
        **+** константа (0/1) в самом конце.  
        Пример: ('K1', 'K2', 'K3', 0)
    known_keys : dict[str, int]
        Биты, определённые однозначно.
    """
    key_eqs: list[tuple] = []
    known_keys: dict[str, int] = {}

    for result, equation in K_equ:
        # предполагаем, что equation[2] – именно вектор участия Ki
        mask = equation[2]
        key_indices = [i + 1 + block_offset for i, bit in enumerate(mask) if bit]

        if not key_indices:
            # уравнение вида 0 = result  (здесь нет Ki) – пропускаем,
            # либо можно проверить на совместность (result должен быть 0)
            continue

        if len(key_indices) == 1:
            # простейший случай Ki = b
            known_keys[f"K{key_indices[0]}"] = result & 1
        else:
            # произвольная длина: K{i1} ⊕ … ⊕ K{in} = b
            vars_ = [f"K{idx}" for idx in key_indices]
            key_eqs.append(tuple(vars_ + [result & 1]))

    return key_eqs, known_keys

def solve_key_equations(key_eqs: List[Tuple[str, ...]],
                        known_keys: Dict[str, int] | None = None):
    """
    Решает систему линейных уравнений над F2 вида
        K_a ⊕ K_b ⊕ … = r

    Parameters
    ----------
    key_eqs : list[tuple]
        Кортеж: (*имена переменных*, result)
        Пример: ('K1','K2','K3', 0)
    known_keys : dict[str,int], optional
        Уже известные биты (K_i: 0/1)

    Returns
    -------
    keys : list[str]
        Отсортированный список всех участвующих K_i
    variants : list[dict[str,int]]
        Все допустимые присвоения {K_i:0/1}, удовлетворяющие системе
    """
    known_keys = known_keys or {}

    # собираем множество всех ключей, встречающихся в уравнениях + известных
    all_keys: set[str] = set().union(*(eq[:-1] for eq in key_eqs)) | known_keys.keys()
    keys = sorted(all_keys)

    # какие ещё неизвестны?
    unknown = [k for k in keys if k not in known_keys]

    variants: list[dict[str, int]] = []

    # перебираем только комбинации для неизвестных битов
    for values in product([0, 1], repeat=len(unknown)):
        current = dict(zip(unknown, values), **known_keys)

        # проверяем каждое уравнение
        ok = True
        for *vars_, res in key_eqs:
            xor_val = 0
            for v in vars_:
                xor_val ^= current[v]
            if xor_val != res:
                ok = False
                break

        if ok:
            variants.append(current)

    return keys, variants

def _block_bit_strings(block, offset, block_size):
    """Возвращает список строк-вариантов для одного блока.
       Если блок полностью неопределён → ['x…x']"""
    key_eqs, known = extract_key_eqs(block, block_offset=offset)
    _, variants    = solve_key_equations(key_eqs, known)
    bit_names      = [f"K{i}" for i in range(offset + 1, offset + block_size + 1)]

    # проверяем: каждый «свободный» бит бывает и 0, и 1?
    fully_free = variants and all(
        len({v.get(b, 'x') for v in variants}) == 2
        for b in bit_names if b not in known
    )

    if fully_free or not variants:          # ограничений нет
        return ['x' * block_size]

    # иначе собираем уникальные строки вида '0110'
    strings = {
        ''.join(str(var.get(b, 'x')) for b in bit_names)
        for var in variants
    }
    return sorted(strings)


def print_key_candidates(blocks, block_size: int = 4):
    """Печатает K1 = …; K2 = …; как в книге."""
    offset = 0
    block_strings = []

    for block in blocks:
        block_strings.append(_block_bit_strings(block, offset, block_size))
        offset += block_size

    # декартово произведение всех блоковых строк
    all_keys = [''.join(parts) for parts in product(*block_strings)]

    for idx, bits in enumerate(all_keys, 1):
        print(f"K{idx} = {bits};")



