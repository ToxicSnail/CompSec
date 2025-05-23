import cipher

S_BOX1 = cipher.S_BOX1
S_BOX2 = cipher.S_BOX2
S_BOX3 = cipher.S_BOX3

def print_table(table, short=0):
    if short == 0:
        columns = [f"{j:>5}" for j in range(1, 8)]
        header = "S(i, j)   |" + "|".join(columns) + "|"
        print(header)
        print("-" * len(header))
        for i in range(15):
            row_bin = format(i + 1, '04b')
            row_vals = "|".join(f"{v:>5}" for v in table[i])
            print(f"{row_bin} ({i + 1:2}) |{row_vals}|")
    else:
        columns = [f"{j:>5}" for j in range(1, 4)]
        header = "S(i, j)  |" + "|".join(columns) + "|"
        print(header)
        print("-" * len(header))
        for i in range(15):
            row_bin = format(i + 1, '04b')
            row_vals = "|".join(f"{v:>5}" for v in table[i])
            print(f"{row_bin} ({i + 1:2}) |{row_vals}|")
    print()

def create_table_sbox1():
    createdtable__sbox = []
    for i in range(1, 16):
        row = []
        for j in range(1, 8):
            positions_i = [pos for pos in range(4) if (i >> pos) & 1]
            positions_j = [pos for pos in range(3) if (j >> pos) & 1]

            count = 0

            for x in range(16):
                bits_i = [(x >> pos) & 1 for pos in positions_i]

                xorBits_i = bits_i[0]
                for bit in bits_i[1:]:
                    xorBits_i ^= bit

                y = cipher.s_box1(x)
                bits_j = [(y >> pos) & 1 for pos in positions_j]

                xorBits_j = bits_j[0]
                for bit in bits_j[1:]:
                    xorBits_j ^= bit
                
                if (xorBits_i == xorBits_j):
                    count += 1

            row.append(count)
        createdtable__sbox.append(row)
    return createdtable__sbox

def create_table_sbox2():
    createdtable__sbox = []
    for i in range(1, 16):
        row = []
        for j in range(1, 8):
            positions_i = [pos for pos in range(4) if (i >> pos) & 1]
            positions_j = [pos for pos in range(3) if (j >> pos) & 1]

            count = 0

            for x in range(16):
                bits_i = [(x >> pos) & 1 for pos in positions_i]

                xorBits_i = bits_i[0]
                for bit in bits_i[1:]:
                    xorBits_i ^= bit

                y = cipher.s_box2(x)
                bits_j = [(y >> pos) & 1 for pos in positions_j]

                xorBits_j = bits_j[0]
                for bit in bits_j[1:]:
                    xorBits_j ^= bit
                
                if (xorBits_i == xorBits_j):
                    count += 1

            row.append(count)
        createdtable__sbox.append(row)
    return createdtable__sbox

def create_table_sbox3():
    createdtable__sbox = []
    for i in range(1, 16):
        row = []
        for j in range(1, 4):
            positions_i = [pos for pos in range(4) if (i >> pos) & 1]
            positions_j = [pos for pos in range(2) if (j >> pos) & 1]

            count = 0

            for x in range(16):
                bits_i = [(x >> pos) & 1 for pos in positions_i]

                xorBits_i = bits_i[0]
                for bit in bits_i[1:]:
                    xorBits_i ^= bit

                y = cipher.s_box3(x)
                bits_j = [(y >> pos) & 1 for pos in positions_j]

                xorBits_j = bits_j[0]
                for bit in bits_j[1:]:
                    xorBits_j ^= bit
                
                if (xorBits_i == xorBits_j):
                    count += 1

            row.append(count)
        createdtable__sbox.append(row)
    return createdtable__sbox

def analyze_sBox(table, flag=0, delta_threshold=0.5):
    first = []
    second = []
    max_value = -1
    second_max_value = -1
    
    if (flag == 0):
        for i in range(15):
            for j in range(7):
                value = table[i][j] / 16
                
                # Фильтрация по порогу дельты
                if abs(1 - 2 * value) >= delta_threshold:
                    value = abs(0.5 - value)
                    if value > max_value:
                        second_max_value = max_value
                        second = first.copy()
                        
                        max_value = value
                        first = [[i + 1, j + 1]]
                    
                    elif value == max_value:
                        first.append([i + 1, j + 1])
                    
                    elif value > second_max_value:
                        second_max_value = value
                        second = [[i + 1, j + 1]]
                    
                    elif value == second_max_value:
                        second.append([i + 1, j + 1])
        return first, second
    elif (flag == 1):
        for i in range(15):
            for j in range(7):
                value = table[i][j] / 16
                
                # Фильтрация по порогу дельты
                if abs(1 - 2 * value) >= delta_threshold:
                    value = abs(0.5 - value)
                    if value > max_value:
                        second_max_value = max_value
                        second = first.copy()
                        
                        max_value = value
                        first = [[i + 1, j + 1]]
                    
                    elif value == max_value:
                        first.append([i + 1, j + 1])
                    
                    elif value > second_max_value:
                        second_max_value = value
                        second = [[i + 1, j + 1]]
                    
                    elif value == second_max_value:
                        second.append([i + 1, j + 1])
        return first, second
    else:
        for i in range(15):
            for j in range(3):
                value = table[i][j] / 16
                
                # Фильтрация по порогу дельты
                if abs(1 - 2 * value) >= delta_threshold:
                    value = abs(0.5 - value)
                    if value > max_value:
                        second_max_value = max_value
                        second = first.copy()
                        
                        max_value = value
                        first = [[i + 1, j + 1]]
                    
                    elif value == max_value:
                        first.append([i + 1, j + 1])
                    
                    elif value > second_max_value:
                        second_max_value = value
                        second = [[i + 1, j + 1]]
                    
                    elif value == second_max_value:
                        second.append([i + 1, j + 1])
        return first, second


CREATED_TABLE_SBOX1 = create_table_sbox1()
CREATED_TABLE_SBOX2 = create_table_sbox2()
CREATED_TABLE_SBOX3 = create_table_sbox3()

print_table(CREATED_TABLE_SBOX1, 0)
print_table(CREATED_TABLE_SBOX2, 1)
print_table(CREATED_TABLE_SBOX3, 2)

# first1, second1 = analyze_sBox(CREATED_TABLE_SBOX1, 0)
# print("лучшие значения S-box 1 = ", first1, "|", second1)
# first2, second2 = analyze_sBox(CREATED_TABLE_SBOX2, 0)
# print("лучшие значения S-box 2 = ", first2, "|", second2)
# first3, second3 = analyze_sBox(CREATED_TABLE_SBOX3, 1)
# print("лучшие значения S-box 3 = ", first3, "|", second3)

