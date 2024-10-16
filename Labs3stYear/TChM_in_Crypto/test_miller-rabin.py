import random

def fi(n):
    f = n
    if n % 2 == 0:
        while n % 2 == 0:
            n = n // 2
        f = f // 2
    i = 3
    while i * i <= n:
        if n % i == 0:
            while n % i == 0:
                n = n // i
            f = f // i
            f = f * (i-1)
        i = i + 2
    if n > 1:
        f = f // n
        f = f * (n-1)
    return f
def test_miller_rabin(n, t):
    if n <= 3:
        print("Число не подходит под условия\n")
        return None
    r = n - 1
    s = 0
    while r % 2 == 0:
        r //= 2
        s += 1
    b = 1
    for _ in range(t):
        b = random.randint(2, n - 2)
        y = pow(b, r, n)
        if (y != 1) and (y != (-1)):
            j = 1
            while j < s and y != n - 1:
                y = pow(y, 2, n)
                if y == 1:
                    return False
                j += 1
            if y != n - 1:
                return False
    return True

def test_ferma(n,t):
    if n <= 3:  print("Число не подходит под условия\n")
    else:
        res = test_ferma_count(n, t)
        if res == 1:
            print("составное [ferma]")
        else:
            print("простое [ferma]")
            error_fi_ferma(n, t)

def test_ferma_count(n, t):
    for i in range(1,t):
        a = random.randint(2, n-2)
        r = pow(a,n-1,n)        #a**(n-1) % n
        if r != 1:  return 1

def error_fi_ferma(n,t):
    i = (fi(n)/n)**t
    #print(fi(n))
    print(f'Вероятность ошибки = {i}')

def error_fi_miller(n, t):
    i = (fi(n) / (4 * n)) ** t
    #print(fi(n))
    #print(f'Вероятность ошибки = {i}')
    print("{:.100f}".format(float(i)))

def test():
    test_int = int(input("Введите число для проверки: "))
    t = int(input("Введите число для проверки: "))

    res = test_miller_rabin(test_int, t)
    if res == True:
        print(f"Число {test_int} простое. [miller]")
        error_fi_miller(test_int, t)
    elif res == False:
        print(f"Число {test_int} составное. [miller]")

    test_ferma(test_int,t)

if __name__ == "__main__":
    test()

#122345761