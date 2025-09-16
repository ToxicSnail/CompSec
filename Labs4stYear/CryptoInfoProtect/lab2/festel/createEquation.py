import createEquationSbox

sbox_equation1 = createEquationSbox.SBOX_EQUATION1
sbox_equation2 = createEquationSbox.SBOX_EQUATION2
sbox_equation3 = createEquationSbox.SBOX_EQUATION3

EX_PERMUTATION = [3, 4, 1, 2, 6, 8, 5, 7, 3, 8, 2, 4]
PERMUTATION = [7, 4, 3, 6, 5, 8, 2, 1]


def koef_equ1(sbox_equ):

    list_equ = []

    x = [0] * 16
    y = [0] * 16
    key = [0] * 4
    for i in range(len(sbox_equ)):
        for k in range(len(sbox_equ[i][0])):
            if sbox_equ[i][0][k] == "1":
                index_X = EX_PERMUTATION[k] + 8
                x[index_X] = 1
                key[k] = 1
        for k in range(len(sbox_equ[i][1]) - 1):
            if sbox_equ[i][1][k + 1] == "1":
                index_Y = PERMUTATION[k]
                y[index_Y] = 1
                
        list_equ.append([x.copy(), y.copy(), key.copy()])
        x = [0] * 16
        y = [0] * 16
        key = [0] * 4

    for i in range(len(list_equ)):
        for k in range(len(list_equ[i][0])):
            if list_equ[i][1][k] == 1:
                list_equ[i][0][k] = 1

    return list_equ

def koef_equ2(sbox_equation2):
    list_equ = []

    x = [0] * 17
    y = [0] * 16
    key = [0] * 4

    for i in range(len(sbox_equation2)):
        for k in range(len(sbox_equation2[i][0])):
            if sbox_equation2[i][0][k] == "1":
                # Берем элементы 6, 8, 5, 7 из EX_PERMUTATION
                index_X = EX_PERMUTATION[k + 4] + 8
                # Проверяем, чтобы index_X не выходил за пределы списка x
                x[index_X] = 1
                key[k] = 1

        for k in range(len(sbox_equation2[i][1]) - 1):
            if sbox_equation2[i][1][k + 1] == "1":
                index_Y = PERMUTATION[k + 3]
                # Проверяем, чтобы index_Y не выходил за пределы списка y
                y[index_Y] = 1

        list_equ.append([x.copy(), y.copy(), key.copy()])
        x = [0] * 17
        y = [0] * 16
        key = [0] * 4

    for i in range(len(list_equ)):
        for k in range(len(list_equ[i][1])):  # Используем длину списка y
            if list_equ[i][1][k] == 1:
                list_equ[i][0][k] = 1

    return list_equ

def koef_equ3(sbox_equation2):
    list_equ = []
    
    # x и y остаются прежнего размера
    x = [0] * 17
    y = [0] * 16
    # key теперь длины 4, чтобы соответствовать sbox_equation2[i][0]
    key = [0] * 4

    for i in range(len(sbox_equation2)):
        # Обработка 4 битов из sbox_equation2[i][0]
        for k in range(4):
            if sbox_equation2[i][0][k] == "1":
                # Для k = 0 используем фиксированный элемент (например, EX_PERMUTATION[8])
                if k == 0:
                    index_X = EX_PERMUTATION[8] + 8
                else:
                    # Для k=1,2,3 используем последние 3 элемента EX_PERMUTATION:
                    # k=1 -> элемент с индексом (len(EX_PERMUTATION)-3) + 0
                    # k=2 -> элемент с индексом (len(EX_PERMUTATION)-3) + 1
                    # k=3 -> элемент с индексом (len(EX_PERMUTATION)-3) + 2
                    index_X = EX_PERMUTATION[(len(EX_PERMUTATION) - 3) + (k - 1)] + 8
                x[index_X] = 1
                key[k] = 1

        # Обработка для PERMUTATION – используем последние 2 элемента.
        # Предполагается, что sbox_equation2[i][1] имеет достаточную длину (как минимум 3 элемента)
        for k in range(2):
            if sbox_equation2[i][1][k + 2] == "1":
                index_Y = PERMUTATION[(len(PERMUTATION) - 2) + k]
                y[index_Y] = 1

        list_equ.append([x.copy(), y.copy(), key.copy()])
        x = [0] * 17
        y = [0] * 16
        key = [0] * 4

    # Объединяем коэффициенты: в x устанавливаем 1 там, где y равен 1
    for i in range(len(list_equ)):
        for k in range(len(list_equ[i][1])):
            if list_equ[i][1][k] == 1:
                list_equ[i][0][k] = 1

    return list_equ


EQU1 = koef_equ1(sbox_equation1)
EQU2 = koef_equ2(sbox_equation2)
EQU3 = koef_equ3(sbox_equation3)

# print("EQU1:", EQU1)
# print("EQU2:",EQU2)
# print("EQU3:",EQU3)
