import pair
import find_key

MASK = 0b1111
KEY = 0b00101010

P_table = (1, 4, 3, 2)
EP_table = (3, 2, 4, 1, 4, 1, 3, 2)
S0_table = (1, 1, 2, 0, 3, 2, 3, 0, 0, 0, 1, 3, 3, 2, 1, 2)
S1_table = (2, 2, 0, 3, 1, 1, 0, 0, 3, 3, 2, 2, 0, 3, 1, 1)

table_of_s_box = {
    "0000": ["01", "10"],
    "0001": ["11", "01"],
    "0010": ["01", "10"],
    "0011": ["10", "01"],
    "0100": ["10", "00"],
    "0101": ["11", "00"],
    "0110": ["00", "11"],
    "0111": ["00", "00"],
    "1000": ["00", "11"],
    "1001": ["11", "00"],
    "1010": ["00", "11"],
    "1011": ["10", "11"],
    "1100": ["01", "10"],
    "1101": ["01", "01"],
    "1110": ["11", "10"],
    "1111": ["10", "01"]
}

def print_keys(sorted_keys, columns=4):
    rows = (len(sorted_freq) + columns - 1) // columns
    data = [sorted_freq[i * rows:(i + 1) * rows] for i in range(columns)]

    print()
    for row in range(rows):
        row_items = []
        for col in range(columns):
            if row < len(data[col]):
                key, count = data[col][row]
                row_items.append(f"{count:>4} : {key}")
            else:
                row_items.append("")
        print("    ".join(row_items))

if __name__ == "__main__":
    slide_pairs = pair.generate_slide_pairs()
    pair.print_pairs_table(slide_pairs)
    
    keys_per_pair = []
    for p in slide_pairs:
        Xp, Yp, X_, Y_ = p

        kx = find_key.subkeys_from_x(Xp, X_)
        ky = find_key.subkeys_from_y(Yp, Y_)

        keys = find_key.intersect_subkeys(kx, ky)
        keys_per_pair.append(keys)

    freq = {}
    for key_list in keys_per_pair:
        for key in key_list:
            freq[key] = freq.get(key, 0) + 1

    sorted_freq = sorted(freq.items(), key=lambda item: item[1], reverse=True)

    print_keys(sorted_freq)
