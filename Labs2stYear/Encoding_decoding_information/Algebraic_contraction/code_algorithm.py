import os
global write_bit
global bit_len
write_bit = 0
bit_len = 8


def index_for_symbol(dictionary, text):
    j = 0
    for i in dictionary:
        if text == i:
            return j + 2
        j += 1

def out_put_bit(bit, output_file):
    global write_bit
    global bit_len
    write_bit >>= 1
    if bit & 1:
        write_bit |= 0x80
    bit_len -= 1
    #print(bit_len)
    if bit_len == 0:
        bit_len = 8
        output_file.write(write_bit.to_bytes(1, "little"))

def bit_plus_follow(bit, bittofollow, outputfile):
    out_put_bit(bit, outputfile)
    for _ in range(bittofollow):
        out_put_bit(~bit, outputfile)

dictionary_sum = 0
with open('input.txt', 'r') as file:
    text_sum = 0
    text = file.read(1)
    dictionary = {}
    while text:
        text_sum += 1
        if dictionary.get(text) == None:
            dictionary.update({text: 1})
        else:
            dictionary[text] = dictionary[text] + 1

        text = file.read(1)
        # print(text)
    # print(dictionary)

    for (symbol,val) in dictionary.items():
        dictionary_sum = dictionary_sum + val
    if text_sum == dictionary_sum:
        print("     File read successfully")
    else:
        print("Couldn't write to file")
        exit()

dictionary = dict(sorted(dictionary.items(), key=lambda item: item[1], reverse=True))

work_interval = [0, 1]
for i in dictionary:
    work_interval.append(dictionary[i] + work_interval[-1])

file = open("coded_text.txt", "wb+")
# print(len(dictionary))
file.write(len(dictionary).to_bytes(1, "little"))
for i in dictionary:
    file.write(i.encode("utf-8"))
    file.write(dictionary[i].to_bytes(4, "little"))
# print(dictionary)

with open('input.txt', 'r') as f:   # алгоритм кодирования
    low_interval = 0
    high_interval = (1 << 16) - 1
    delete = work_interval[-1]
    diff = high_interval - low_interval + 1
    first_q = int(int(high_interval + 1) / 4)
    half_q = first_q * 2
    third_q = first_q * 3
    bit_to_follow = 0

    test = f.read(1)
    while test:
        index = index_for_symbol(dictionary, test)
        high_interval = int(low_interval + work_interval[index] * diff / delete - 1)
        low_interval = int(low_interval + work_interval[index - 1] * diff / delete)

        while True:
            if high_interval < half_q:
                bit_plus_follow(0, bit_to_follow, file)
                bit_to_follow = 0
            elif low_interval >= half_q:
                bit_plus_follow(1, bit_to_follow, file)
                bit_to_follow = 0
                low_interval -= half_q
                high_interval -= half_q
            elif low_interval >= first_q and high_interval < third_q:
                bit_to_follow += 1
                low_interval -= first_q
                high_interval -= first_q
            else:
                break
            low_interval += low_interval
            high_interval += high_interval + 1

        diff = high_interval - low_interval + 1
        test = f.read(1)

    high_interval = int(low_interval + work_interval[1] * diff / delete - 1)
    low_interval = int(low_interval + work_interval[0] * diff / delete)

    while True:
        if high_interval < half_q:
            bit_plus_follow(0, bit_to_follow, file)
            bit_to_follow = 0
        elif low_interval >= half_q:
            bit_plus_follow(1, bit_to_follow, file)
            bit_to_follow = 0
            low_interval -= half_q
            high_interval -= half_q
        elif low_interval >= first_q and high_interval < third_q:
            bit_to_follow += 1
            low_interval -= first_q
            high_interval -= first_q
        else:
            break
        low_interval += low_interval
        high_interval += high_interval + 1
    bit_to_follow += 1
    if low_interval < first_q:
        bit_plus_follow(0, bit_to_follow, file)
        bit_to_follow = 0
    else:
        bit_plus_follow(1, bit_to_follow, file)
        bit_to_follow = 0

    write_bit >>= bit_len
    file.write(write_bit.to_bytes(1, "little"))

file.close()
print("U file has been successful coded")
