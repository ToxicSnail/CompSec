import time
import sys


sys.set_int_max_str_digits(1000000)


def algor(x, y):  # Дихотомический алгоритм возведения в степень
    z = 1
    while y > 0:
        if y % 2 == 1:
            z *= x
        x *= x
        y //= 2
    return z

def standart(x, y):  # Стандартный алгоритм возведения в степень
    res = 1
    for i in range(y): 
        res *= x
    return res

def test_algorithms():
    x = 121348152431231287936219
    y = 1235
    repetitions = 10  # Количество повторений для усреднения времени выполнения
    
    total_time_algor = 0
    total_time_standard = 0
    
    for _ in range(repetitions):
        start = time.time_ns() 
        result_1 = algor(x, y)
        total_time_algor += time.time_ns() - start

        start = time.time_ns()
        result_2 = standart(x, y)
        total_time_standard += time.time_ns() - start

        # Проверяем результаты внутри цикла повторений
        if result_1 != result_2:
            print("Results do not match")
            return
    
    avg_time_algor = total_time_algor / repetitions
    avg_time_standard = total_time_standard / repetitions
    
    # print(result_1)
    # print(result_2)
    print(f'Algor: {avg_time_algor} ns')
    print(f'Standard: {avg_time_standard} ns')

if __name__ == '__main__':
    test_algorithms()
