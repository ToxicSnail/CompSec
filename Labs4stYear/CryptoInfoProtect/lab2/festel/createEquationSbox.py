import sboxAnalize

first_max_p1, second_max_p1 = sboxAnalize.analyze_sBox(sboxAnalize.CREATED_TABLE_SBOX1, 0, 0.625)
first_max_p2, second_max_p2 = sboxAnalize.analyze_sBox(sboxAnalize.CREATED_TABLE_SBOX2, 1)
first_max_p3, second_max_p3 = sboxAnalize.analyze_sBox(sboxAnalize.CREATED_TABLE_SBOX3, 2, 0.25)


def get_probability(first_max, second_max, table_sbox):

    pr = []
    unification_list = [i for i in first_max]
    unification_list.extend([i for i in second_max])

    for i in range(len(unification_list)):
        a = unification_list[i][0]
        b = unification_list[i][1]
        value = table_sbox[a - 1][b - 1]
        pr.append(value)
        
    return pr

PR1 = get_probability(first_max_p1, second_max_p1, sboxAnalize.CREATED_TABLE_SBOX1)
PR2 = get_probability(first_max_p2, second_max_p2, sboxAnalize.CREATED_TABLE_SBOX2)
PR3 = get_probability(first_max_p3, second_max_p3, sboxAnalize.CREATED_TABLE_SBOX3)

def calculate_delta(pr_values, d = 0.5):
    # Список для хранения значений, которые соответствуют условию delta >= 1/2
    valid_deltas = []
    
    for value in pr_values:
        # Вычисляем p
        p = value / 16
        # Вычисляем дельту
        delta = abs(1 - 2 * p)
        # Если дельта >= 1/2, добавляем в список
        if delta >= d:
            valid_deltas.append(delta)
    
    return valid_deltas

# Применение функции для PR1, PR2, PR3
valid_deltas_1 = calculate_delta(PR1, 0.625)
valid_deltas_2 = calculate_delta(PR2)
valid_deltas_3 = calculate_delta(PR3, 0.25)

print("\n")
print("PR1:",PR1, ", его Дельты:", valid_deltas_1)
print("PR2:",PR2, ", его Дельты:", valid_deltas_2)
print("PR3:",PR3, ", его Дельты:", valid_deltas_3)

def unification_bin(first_max, second_max):
    unification_list = [i for i in first_max]
    unification_list.extend([i for i in second_max])
    unification_list_bin = []
    for i in range(len(unification_list)):
        tmp = [f"{j:04b}" for j in unification_list[i]]
        unification_list_bin.append(tmp)

    return unification_list_bin

def print_sbox_equation(unification_list_bin):
    for i in range(len(unification_list_bin)):
        x = ""
        y = ""
        right = ""
        result = ""

        for k in range(0, 4):
            if unification_list_bin[i][0][k] == "1":
                if x:
                    x += " + "
                x += f"X{k + 1}"
                right += f"K{k + 1} + "

        for k in range(0, 3):
            if unification_list_bin[i][1][k + 1] == "1":
                if y:
                    y += " + "
                y += f"Y{k + 1}"

        right = right.rstrip(" + ")

        result = f"{x} + {y} = {right}"
        print(result)

SBOX_EQUATION1 = unification_bin(first_max_p1, second_max_p1)
SBOX_EQUATION2 = unification_bin(first_max_p2, second_max_p2)
SBOX_EQUATION3 = unification_bin(first_max_p3, second_max_p3)

print("\n")
# print(SBOX_EQUATION1)
# print_sbox_equation(SBOX_EQUATION1)
# print(SBOX_EQUATION2)
# print_sbox_equation(SBOX_EQUATION2)
# print(SBOX_EQUATION3)
# print_sbox_equation(SBOX_EQUATION3)