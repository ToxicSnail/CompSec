import random
import string

def gen_key():
    passwd = []

    passwd += ([random.choice([c, c.upper()]) for c in "exam"] + ["{"])
    passwd += (random.sample(['3', 'l', '1', 't'], 4) + ["_"])

    c1 = random.choice(string.printable)
    c2 = random.choice(string.printable)
    passwd += [c1, c2, c2, c1, "_"]

    check4 = ["1337", "4242", "DEAD", "BEEF", "CAFE", "BABE"]   #const may be lower_case
    passwd += list(random.choice(check4)) + ["_"]

    tmp = ''.join(c for c in string.ascii_uppercase if c not in 'AEIOU')

    passwd += random.choice(tmp)    # todo
    passwd += random.choice('AEIOU')
    passwd += random.choice(tmp)
    passwd += random.choice('AEIOU')

    passwd += "_ROCK}"

    return ''.join(passwd)
    
for i in range(10):
    print(gen_key())
