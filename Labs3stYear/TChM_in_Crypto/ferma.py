import random

def fi(n):
    f = n
    if n%2 == 0:
        while n%2 == 0:
            n = n // 2
        f = f // 2
    i = 3
    while i*i <= n:
        if n%i == 0:
            while n%i == 0:
                n = n // i
            f = f // i
            f = f * (i-1)
        i = i + 2
    if n > 1:
        f = f // n
        f = f * (n-1)
    return f

def error_fi(n,t):
    i = (fi(n)/n)**t
    print(fi(n))
    print(f'Вероятность ошибки = {i}')

def test_ferma_count(n, t):
    for i in range(1,t):
        a = random.randint(2, n-2)
        r = pow(a,n-1,n)        #a**(n-1) % n
        if r != 1:  return 1


def test_ferma(n,t):
    if n <= 3:  print("Число не подходит под условия\n")
    else:
        res = test_ferma_count(n, t)
        if res == 1:
            print("составное")
        else:
            print("простое")
            error_fi(n, t)


def main():
    n = int(input("Введите число для проверки: "))
    t = int(input("Введите параметр надёжности: "))
    test_ferma(n,t)

if __name__ == "__main__":
    main()
