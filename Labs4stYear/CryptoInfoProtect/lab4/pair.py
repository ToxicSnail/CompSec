import DES

MASK = 0b1010
KEY = 0b10001001

pairs = []

def generate_slide_pairs():
    """Генерирует слайдовые пары (X, X', Y, Y'), удовлетворяющие условиям атаки."""
    pairs = []
    for i in range(2**8):
        for j in range(2**8):
            pt1_bin = f"{i:08b}"
            pt2_bin = f"{j:08b}"

            # Проверка условия слайдовой пары:
            # 1. Правая половина X' == левая половина X == MASK
            # 2. Левая половина Y' == правая половина Y
            if pt1_bin[4:] == pt2_bin[:4] == f"{MASK:04b}":
                ct1 = DES.encrypt(KEY, i)
                ct2 = DES.encrypt(KEY, j)

                ct1_bin = f"{ct1:08b}"
                ct2_bin = f"{ct2:08b}"

                if ct1_bin[:4] == ct2_bin[4:]:
                    pairs.append((pt1_bin, ct1_bin, pt2_bin, ct2_bin))
    return pairs

def print_pairs_table(pairs):
    """Выводит слайдовые пары в виде таблицы."""
    if not pairs:
        print("Слайдовые пары не найдены!")
        return

    print("\nСлайдовые пары:")
    print("-" * 51)
    print(f"| {'№':<3} | {'X':<8} | {'X\'':<8} | {'Y':<8} | {'Y\'':<8} |")
    print("-" * 51)
    
    for idx, (x, y, x_prime, y_prime) in enumerate(pairs, 1):
        print(f"| {idx:<3} | {x} | {x_prime} | {y} | {y_prime} |")
    print("-" * 51)


        