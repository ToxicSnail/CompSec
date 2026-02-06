import random
import string

def gen_key():
    passwd = []

    passwd += ([random.choice([c, c.upper()]) for c in "exam"] + ["{"])
    passwd += (random.sample(['3', 'l', '1', 't'], 4) + ["_"])

    c1 = random.choice(string.printable)
    c2 = random.choice(string.printable)
    passwd += [c1, c2, c2, c1, "_"]

    check4 = ["1337", "4242", "DEAD", "BEEF", "CAFE", "BABE"]
    word = random.choice(check4)
    rand_case_word = [
        random.choice((ch.lower(), ch.upper()))
        for ch in word
    ]
    passwd += rand_case_word + ["_"]

    # tmp = ''.join(c for c in string.ascii_lowercase if c not in 'aeiou')
    # passwd += random.choice(tmp)
    # passwd += random.choice('aeiou')
    # passwd += random.choice(tmp)
    # passwd += random.choice('aeiou')
    vowels = "OIEAU"
    not_vowels   = ''.join(c for c in string.ascii_uppercase if c not in vowels)
    def pick(letter):
        ch = random.choice(letter)
        if random.getrandbits(1):
            return ch.lower()
        else:
            return ch
    check5 = ''.join(pick(letter) for letter in (not_vowels, vowels, not_vowels, vowels))
    passwd += check5

    passwd += "_ROCK}"

    return ''.join(passwd)
    
for i in range(10):
    print(gen_key())
