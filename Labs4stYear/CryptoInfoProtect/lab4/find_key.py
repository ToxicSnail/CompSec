import main

def expand_block(four_bits: str) -> str:
    result = []
    for pos in main.EP_table:
        result.append(four_bits[pos - 1])
    return ''.join(result)

def permute_block(four_bits: str) -> str:
    result = []
    for pos in main.P_table:
        result.append(four_bits[pos - 1])
    return ''.join(result)


def subkeys_from_x(xprime: str, x: str):
    rx_bin = x[4:]
    lxp_bin = xprime[:4]

    rx_val = int(rx_bin, 2)
    lxp_val = int(lxp_bin, 2)
    f_val = rx_val ^ lxp_val
    f_bin = f"{f_val:04b}"

    s1 = f_bin[:2]
    s2 = f_bin[2:]

    right_xp = xprime[4:]
    expanded_str = expand_block(right_xp)

    input_sbox1 = expanded_str[:4]
    input_sbox2 = expanded_str[4:]

    s1_candidates = []
    s2_candidates = []
    for key_4bits, (out_s1, out_s2) in main.table_of_s_box.items():
        if out_s1 == s1:
            s1_candidates.append(key_4bits)
        if out_s2 == s2:
            s2_candidates.append(key_4bits)

    sub_key1 = []
    sub_key2 = []
    for c_s1 in s1_candidates:
        for c_s2 in s2_candidates:
            k1 = int(input_sbox1, 2) ^ int(c_s1, 2)
            k2 = int(input_sbox2, 2) ^ int(c_s2, 2)
            sub_key1.append(f"{k1:04b}")
            sub_key2.append(f"{k2:04b}")

    return sub_key1, sub_key2

def subkeys_from_y(yprime: str, y: str):
    yr_p_bin = yprime[4:]
    yl_bin   = y[:4]

    yr_p_val = int(yr_p_bin, 2)
    y_l_val  = int(yl_bin, 2)
    f_val = yr_p_val ^ y_l_val
    f_bin = f"{f_val:04b}"

    perm4 = permute_block(f_bin)
    s1 = perm4[:2]
    s2 = perm4[2:]

    yl_p = yprime[:4]
    expanded_str = expand_block(yl_p)

    input_sbox1 = expanded_str[:4]
    input_sbox2 = expanded_str[4:]

    s1_candidates = []
    s2_candidates = []
    for key_4bits, (out_s1, out_s2) in main.table_of_s_box.items():
        if out_s1 == s1:
            s1_candidates.append(key_4bits)
        if out_s2 == s2:
            s2_candidates.append(key_4bits)

    sub_key1 = []
    sub_key2 = []
    for c_s1 in s1_candidates:
        for c_s2 in s2_candidates:
            k1 = int(input_sbox1, 2) ^ int(c_s1, 2)
            k2 = int(input_sbox2, 2) ^ int(c_s2, 2)
            sub_key1.append(f"{k1:04b}")
            sub_key2.append(f"{k2:04b}")

    return sub_key1, sub_key2

def intersect_subkeys(kx, ky):
    k1_x, k2_x = kx
    k1_y, k2_y = ky

    set_k1x = set(k1_x)
    set_k1y = set(k1_y)
    set_k2x = set(k2_x)
    set_k2y = set(k2_y)

    common_k1 = set_k1x.intersection(set_k1y)
    common_k2 = set_k2x.intersection(set_k2y)

    result = []
    for c1 in common_k1:
        for c2 in common_k2:
            result.append(c1 + c2)

    return result
