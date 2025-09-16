// Miller-Rabin.h
#ifndef MILLER_RABIN_H
#define MILLER_RABIN_H

//#include "BigNum.h" // Подключаем BigNum
#include <iostream>
#include <cstdlib>

// Функция Эйлера (phi)
BN phi(BN n) {
    BN result = n;
    BN p;
    BN zero, one;
    zero = 0; one = 1; p = 2;
    while (p * p <= n) {
        if (n % p == zero) {
            while (n % p == zero) {
                n = n / p;
            }
            result = result - result / p;
        }
        p = p + one;
    }
    if (n > one) {
        result = result - result / n;
    }
    return result;
}

// Тест Миллера-Рабина
bool test_miller_rabin(BN n, int t) {
    BN zero, one, two, three;
    zero = 0; one = 1; two = 2; three = 3;
    if (n <= three) {
        cout << "Test int must be more than 3" << endl;
        return false;
    }
    BN r = n - one;
    BN s = zero;
    while (r % two == zero) {
        r = r / two;
        s = s + one;
    }
    for (int i = 0; i < t; i++) {
        BN tmp;
        tmp = rand();
        BN b = two + tmp % (n - three); // Случайное число в диапазоне [2, n-2]
        BN y = b.pow_mod(r, n); // Используем метод возведения в степень по модулю из BigNum
        if (y != one) {
            BN j = one;
            while (j < s && y != n - one) {
                y = y * y % n;
                if (y == one) {
                    return false;
                }
                j = j + one;
            }
            if (y != n - one) {
                return false;
            }
        }
    }
    return true;
}

//Оценка вероятности ошибки
// void error_phi(BN n, int t) {
//     BN phi_val = phi(n);
//     double X = pow(phi_val.to_double() / (4 * n.to_double()), t);
//     cout << "Вероятность ошибки = " << X << endl;
// }


#endif // MILLER_RABIN_H
