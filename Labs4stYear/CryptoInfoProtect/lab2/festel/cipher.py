import main

EXTENDED_PERMUTATION = main.EXTENDED_PERMUTATION
PERMUTATION = main.PERMUTATION

S_BOX1 = main.S_BOX1
S_BOX2 = main.S_BOX2
S_BOX3 = main.S_BOX3

def extended_permutation(block):
    bit = f"{block:08b}"
    permute_bit = [bit[i] for i in EXTENDED_PERMUTATION]
    permute_bit = ''.join(permute_bit)
    return int(permute_bit, 2)

def permute(block):
    bit = f"{block:08b}"
    permute_bit = [bit[i] for i in PERMUTATION]
    permute_bit = ''.join(permute_bit)
    return int(permute_bit, 2)

def xor(block, key):
    return block ^ key

def s_box1(block):
    a1 = int(f"{block:04b}"[0], 2)
    a2 = int(f"{block:04b}"[1:], 2)

    return S_BOX1[a1][a2]

def s_box2(block):
    a1 = int(f"{block:04b}"[0], 2)
    a2 = int(f"{block:04b}"[1:], 2)

    return S_BOX2[a1][a2]

def s_box3(block):
    a1 = int(f"{block:04b}"[0] + f"{block:04b}"[3], 2)
    a2 = int(f"{block:04b}"[1] + f"{block:04b}"[2], 2)
    return S_BOX3[a1][a2]


def f_k(block, key):
    extended_block = extended_permutation(block)
    xor_block = xor(extended_block, key)
    xor_block = f"{xor_block:012b}"
    res_sbox1 = s_box1(int(xor_block[:4], 2))
    res_sbox2 = s_box2(int(xor_block[4:8], 2))
    res_sbox3 = s_box3(int(xor_block[8:], 2))
    result_sbox = f"{res_sbox1:03b}" + f"{res_sbox2:03b}" + f"{res_sbox3:02b}"
    result = permute(int(result_sbox, 2))
    return result


def encrypt(pt, key):
    bit_str = f"{pt:016b}"  # Ensure the binary string is 16 bits long
    
    l = int(bit_str[0:8], 2)  # First 8 bits
    r = int(bit_str[8:], 2)   # Last 8 bits
    
    result_f = f_k(r, key)
    new_l = xor(result_f, l)
    new_r = r
    
    result = int(f"{new_l:08b}" + f"{new_r:08b}", 2)
    return result