global read_bit
global bit_len
global garbage_bit
read_bit = 0
bit_len = 0
garbage_bit = 0


def in_put_bit(input_file):
    global read_bit
    global bit_len
    global garbage_bit
    if bit_len == 0:
        sid_bit = input_file.read(1)
        read_bit = int.from_bytes(sid_bit, "little")
        if sid_bit == b"":
            garbage_bit += 1
            read_bit = 255
            if garbage_bit > 14:
                print("File hasn't been decoded")
                exit(1)
        bit_len = 8

    t = read_bit & 1
    read_bit >>= 1
    bit_len -= 1
    return t


with open("coded_text.txt", "rb") as coded_text_file:
    text = ord(coded_text_file.read(1))
    # print(text)
    dictionary = {}
    for i in range(text):
        key_header = coded_text_file.read(1).decode('utf-8')
        #print(key_header)
        value_header = int.from_bytes(coded_text_file.read(4), "little")
        #print(value_header)
        dictionary[key_header] = value_header
    # print(dictionary)
    print("     File read successfully")

    work_interval = [0, 1]
    for i in dictionary:
        work_interval.append(dictionary[i] + work_interval[-1])
    # print(work_interval)

    with open("output.txt", "wb+") as output_file:
        low_interval = 0
        high_interval = (1 << 16) - 1  # 2^16
        delete = work_interval[-1]
        diff = high_interval - low_interval + 1
        first_q = int(int(high_interval + 1) / 4)
        half_q = first_q * 2
        third_q = first_q * 3
        val = 0

        for i in range(16):
            k = in_put_bit(coded_text_file)
            val += val + k
        while True:
            freq = int(((val - low_interval + 1) * delete - 1) / diff)
            j = 1
            while work_interval[j] <= freq:
                j += 1
            high_interval = int(low_interval + work_interval[j] * diff / delete - 1)
            low_interval = int(low_interval + work_interval[j - 1] * diff / delete)

            while True:
                if high_interval < half_q:
                    pass
                elif low_interval >= half_q:
                    low_interval -= half_q
                    high_interval -= half_q
                    val -= half_q
                elif low_interval >= first_q and high_interval < third_q:
                    low_interval -= first_q
                    high_interval -= first_q
                    val -= first_q
                else:
                    break
                low_interval += low_interval
                high_interval += high_interval + 1
                k = in_put_bit(coded_text_file)
                val += val + k

            if j == 1:
                break
            output_file.write(list(dictionary.keys())[j - 2].encode('utf-8'))
            diff = high_interval - low_interval + 1

output_file.close()
print("U file has been successful decoded")
