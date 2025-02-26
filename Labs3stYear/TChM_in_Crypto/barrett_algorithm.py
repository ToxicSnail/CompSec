import time
def barrett_algorithm(m, x, b, k, z):
    if len(str(x)) > 2*len(str(m)):
        print("x bigger")
        return 0
    if b > 3:
        q = ((x // b ** (k - 1)) * z) // b ** (k + 1)
        r_1 = x % b ** (k + 1)
        r_2 = (q * m) % b ** (k + 1)
        if r_1 >= r_2:
            r = r_1 - r_2
        else:
            r = b ** (k + 1) + r_1 - r_2
        while(r >= m):
            r -= m
    else:
        print('b must be bigger')
    return r

def standart(x,m):
    while x >= m:
        x-=m
    return x

def test (m, x, b):
    k = len(str(m))
    z = b ** (2 * k) // m
    start = time.time_ns()
    result_1 = barrett_algorithm(m, x, b, k, z)
    #print(f'{time.time_ns() - start} ns')
    
    start = time.time_ns()
    result_2 = standart(x, m)
    #print(f'{time.time_ns() - start} ns')
    
    if (result_1  == result_2):
        print('Result Normal')
        print(result_1)

    else:
        print('Result not Normal')



if __name__ == '__main__':

    m = int(input("Введите m:  "))
    x = int(input("Введите x:  "))
    b = int(input("Введите b:  "))
    test(m,x,b) 

    # x = [1234567,98765432,6364346634, 35234623,4213435654,5736474, 12345678235]
    # m = 3000
    # b = 10
    # for i in x:
    #     test (m, i, b)

