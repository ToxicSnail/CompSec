from itertools import product
import random

# Таблицы перестановок и замен
IP = [1, 5, 2, 0, 3, 7, 4, 6]
IP_INV = [3, 0, 2, 4, 6, 1, 7, 5]
EP = [3, 0, 1, 2, 1, 2, 3, 0]
P4 = [1, 3, 2, 0]
P10 = [2, 4, 1, 6, 3, 9, 0, 8, 7, 5]
P8 = [5, 2, 6, 3, 7, 4, 9, 8]

# S-блоки
S0 = [[1, 0, 3, 2], [3, 2, 1, 0], [0, 2, 1, 3], [3, 1, 3, 2]]
S1 = [[0, 1, 2, 3], [2, 0, 1, 3], [3, 0, 1, 0], [2, 1, 0, 3]]

def permute(bits, perm_table):
    return [bits[i] for i in perm_table]

def left_shift(bits, n):
    return bits[n:] + bits[:n]

def key_schedule(key):
    key = permute(key, P10)
    left, right = key[:5], key[5:]
    
    left, right = left_shift(left, 1), left_shift(right, 1)
    k1 = permute(left + right, P8)

    left, right = left_shift(left, 2), left_shift(right, 2)
    k2 = permute(left + right, P8)
    
    return k1, k2

def s_box(bits, sbox):
    row = (bits[0] << 1) + bits[3]
    col = (bits[1] << 1) + bits[2]
    return [int(x) for x in format(sbox[row][col], '02b')]

def f_function(bits, key):
    bits = permute(bits, EP)
    bits = [bits[i] ^ key[i] for i in range(8)]
    
    left, right = bits[:4], bits[4:]
    left, right = s_box(left, S0), s_box(right, S1)
    
    return permute(left + right, P4)

def round_function(bits, key):
    left, right = bits[:4], bits[4:]
    left = [left[i] ^ f_function(right, key)[i] for i in range(4)]
    return left + right

def encrypt(plain, key1, key2):
    bits = permute(plain, IP)
    bits = round_function(bits, key1)
    bits = bits[4:] + bits[:4]
    bits = round_function(bits, key2)
    return permute(bits, IP_INV)

def decrypt(cipher, key1, key2):
    bits = permute(cipher, IP)
    bits = round_function(bits, key2)
    bits = bits[4:] + bits[:4]
    bits = round_function(bits, key1)
    return permute(bits, IP_INV)

# MITM-attack
def meet_in_the_middle_attack(M, C, filtered_keys=None):
    possible_keys = list(product([0, 1], repeat=10)) if filtered_keys is None else filtered_keys
    y1 = {}

    # Y1 = S-DES(M, K1)
    for k1 in possible_keys:
        k1_1, k1_2 = key_schedule(k1)
        Y1 = encrypt(M, k1_1, k1_2)
        Y1 = tuple(Y1) 
        if Y1 not in y1:
            y1[Y1] = list()
        y1[Y1].append(k1)

    # Y2 = S-DES^-1(C, K2)
    candidates = []
    for k2 in possible_keys:
        k2_1, k2_2 = key_schedule(k2)
        Y2 = decrypt(C, k2_1, k2_2)
        Y2 = tuple(Y2)
        if Y2 in y1:
            candidates.append((y1[Y2], k2))

    # перезапускаем атаку с новым случайным M'
    while len(candidates) > 1:
        
        M_prime = [random.randint(0, 1) for _ in range(8)]
        print(f" New random plaintext: {M_prime}")

        C_prime = encrypt(encrypt(M_prime, key_schedule(key1)[0], key_schedule(key1)[1]),
            key_schedule(key2)[0], key_schedule(key2)[1])

        filtered_candidates = []
        for k1_list, k2 in candidates:
            k1_set = []
            for k1 in k1_list:
                if validate_keys(M_prime, C_prime, k1, k2):
                    k1_set.append(k1)
            if len(k1_set) > 0:
                filtered_candidates.append([k1_set, k2])

        if len(filtered_candidates) == 1:
            return filtered_candidates[0]
        
        candidates = filtered_candidates
    return candidates[0] if candidates else None

def validate_keys(M, C, k1, k2):
    k1_1, k1_2 = key_schedule(k1)
    k2_1, k2_2 = key_schedule(k2)

    C_check = encrypt(encrypt(M, k1_1, k1_2), k2_1, k2_2)

    # print(f" Проверка: {C_check} == {C}?")

    return C_check == C  

if __name__ == "__main__":
    # plaintext = [1, 0, 1, 1, 0, 1, 1, 0] 
    # key1 = [0, 0, 1, 1, 0, 1, 1, 1, 1, 1]
    # key2 = [1, 0, 1, 1, 0, 1, 0, 0, 1, 1]

    plaintext = [random.randint(0, 1) for _ in range(8)]
    key1 = [random.randint(0, 1) for _ in range(10)]
    key2 = [random.randint(0, 1) for _ in range(10)]


    k1_1, k1_2 = key_schedule(key1)
    k2_1, k2_2 = key_schedule(key2)

    ciphertext = encrypt(encrypt(plaintext, k1_1, k1_2), k2_1, k2_2)

    decrypted_text = decrypt(decrypt(ciphertext, k2_1, k2_2), k1_1, k1_2)

    print("# S-DES Double Encryption/Decryption")
    print(f" Plaintext:           {plaintext}")
    print(f" Key 1 (K1):          {key1}")
    print(f" Key 2 (K2):          {key2}")
    print(f" Ciphertext:          {ciphertext}")
    print(f" Decrypted Text:      {decrypted_text}")

    if decrypted_text == plaintext:
        print("Decryption was successful!\nThe decrypted text matches the original plaintext.")
    else:
        print("Decryption failed! The decrypted text does not match the original plaintext.")

    print("\n# Meet-in-the-Middle Attack")
    keys_found = meet_in_the_middle_attack(plaintext, ciphertext)

    if keys_found:
        print("Found Key Pair")
        print(f"  Found K1: {keys_found[0]}")
        print(f"  Found K2: {keys_found[1]}")

        if keys_found == [[tuple(key1)], tuple(key2)]:
            print("The attack successfully found the correct keys!")
        else:
            print("The found keys do not match the original ones!")
    else:
        print("Keys not found during the attack.")
