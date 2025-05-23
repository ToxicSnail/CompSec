import createEquation
import createEquationSbox
import random
from findK import *

N = 100 # Количество пар открытый/закрытый текстов
# KEY = 0b100011001010
# KEY = random.getrandbits(12)
KEY_LENGTH = 12 

EXTENDED_PERMUTATION = [2, 3, 0, 1, 5, 7, 4, 6, 2, 7, 1, 3] # перестановка 8-битного блока в 12 бит
PERMUTATION = [7, 6, 2, 1, 4, 3, 0, 5]  

S_BOX1 = [
    [6, 2, 7, 4, 1, 1, 2, 3],
    [5, 1, 2, 5, 3, 4, 1, 6]
]

S_BOX2 = [
    [6, 5, 3, 5, 7, 1, 2, 2],
    [5, 1, 6, 4, 6, 3, 4, 7]
]

S_BOX3 = [
    [3, 2, 1, 3],
    [2, 1, 3, 2],
    [1, 3, 2, 1],
    [3, 2, 1, 3]
]

def main():

    # print(f"\nСекретный ключ: {KEY:0{KEY_LENGTH}b}\n")
    equ1 = createEquation.EQU1
    equ2 = createEquation.EQU2
    equ3 = createEquation.EQU3

    pr1 = createEquationSbox.PR1
    pr2 = createEquationSbox.PR2
    pr3 = createEquationSbox.PR3

    max_equation_length = max(
        max(len(eq[0]), len(eq[1])) for eq in equ1 + equ2 + equ3
    )

    selection = generate_selection(max_equation_length, N)

    T1_vals = make_T(selection, equ1, N)
    T2_vals = make_T(selection, equ2, N)
    T3_vals = make_T(selection, equ3, N)

    T1 = list(zip(T1_vals, equ1))
    T2 = list(zip(T2_vals, equ2))
    T3 = list(zip(T3_vals, equ3))

    # print_sample_texts(selection)

    left_part1 = make_left_part(pr1, T1_vals, N)
    left_part2 = make_left_part(pr2, T2_vals, N)
    left_part3 = make_left_part(pr3, T3_vals, N)

    K_EQU1 = make_K(left_part1, equ1)
    K_EQU2 = make_K(left_part2, equ2)
    K_EQU3 = make_K(left_part3, equ3)

    print_equation_table(1, T1, pr1, K_EQU1, N)
    print_equation_table(2, T2, pr2, K_EQU2, N)
    print_equation_table(3, T3, pr3, K_EQU3, N)
    
    print("\n\n")
    print_key_candidates([K_EQU1, K_EQU2, K_EQU3])



if __name__ == "__main__":
    main()
