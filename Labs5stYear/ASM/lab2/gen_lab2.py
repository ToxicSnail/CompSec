import z3

string = [z3.BitVec(f"c{i}", 8) for i in range(20)]
solver = z3.Solver()

def subCh(c):
    return z3.Or(
        z3.And(c >= ord('0'), c <= ord('9')),
        z3.And(c >= ord('A'), c <= ord('Z')),
        z3.And(c >= ord('a'), c <= ord('z')),
    )

for i in range(20):
    c = string[i]

    if i == 0:
        solver.add(c == ord('('))
    elif i == 5:
        solver.add(c == ord('{'))
    elif i == 14:
        solver.add(c == ord('}'))
    elif i == 19:
        solver.add(c == ord(')'))
    else:
        solver.add(subCh(c))

v6 = []
for i in range(20):
    if i in (0, 5, 14, 19):
        continue
    v6.append(string[i])
assert len(v6) == 16


def ch1(string, count):
    v3 = z3.BitVecVal(1, 32)
    for i in range(count):
        v3 *= z3.ZeroExt(24, string[i])
    return v3 % 126

def ch2(string):
    v = string[3] | ((string[2] + string[1]) ^ string[0])
    res = (z3.ZeroExt(24, v)) <<  8          
    return res                     

def ch3(string, count):
    v3 = z3.BitVecVal(0, 32)
    for i in range(count):
        v3 = 2 * (z3.BitVecVal(i, 32) ^ z3.ZeroExt(24, string[i]) ^ v3 ^ z3.BitVecVal(8, 32))
    return v3 ^ z3.BitVecVal(0x5A, 32)

def ch4(string):
    x, y, z, w = string
    return z3.And(
        z == (y ^ x),
        w == (z ^ y),
    )

solver.add(ch1(v6[0:4], 4) == 0x50)
solver.add(ch2(v6[4:8]) == 0x6E00)
solver.add(ch3(v6[8:12], 4) == 0x4E)
solver.add(ch4(v6[12:16]))


for i in range(10):
    if solver.check() != z3.sat:
        break

    model = solver.model()

    key_bytes = [model[c].as_long() for c in string]
    key = ''.join(chr(b) for b in key_bytes)
    print(key)



    solver.add(z3.Not(z3.And((string[i] == key_bytes[i] for i in range(20)))))
else:
    print("-_-")
