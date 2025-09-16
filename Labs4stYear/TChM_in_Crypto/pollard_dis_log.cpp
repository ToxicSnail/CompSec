#include <iostream>
#include <tuple>
#include "BigNumber.h"

// Функция модульного вычитания
BN mod_sub(const BN& a, const BN& b, const BN& n) {
    if (a >= b) {
        return (a - b) % n;
    } else {
        // std::cout << "DEBUG else " << a << " - " << b << " mod " << n << "\n";
        std::cout << "вот попали в else, щас попробуем вывести отладочные принты\n";
        std::cout << "DEBUG " << b << "\n";
        return (n + a - b) % n;
    }
}

// Функции для работы с множествами S1, S2, S3
bool in_S1(const BN& x) {
    return x % 3 == 1;
}

bool in_S2(const BN& x) {
    return x % 3 == 2;
}

bool in_S3(const BN& x) {
    return x % 3 == 0;
}

// Функция F для вычисления следующего шага
std::tuple<BN, BN, BN> F(const BN& x, const BN& y, const BN& b, const BN& g, const BN& a, const BN& p) {
    BN new_x, new_u, new_v;
    BN one = 1;
    BN two = 2;

    if (in_S1(x)) {
        new_x = (a * x) % p;
        new_u = y;
        new_v = (b + one) % (p - one);
    } else if (in_S2(x)) {
        new_x = (x * x) % p;
        new_u = (two * y) % (p - one);
        new_v = (two * b) % (p - one);
    } else if (in_S3(x)) {
        new_x = (g * x) % p;
        new_u = (y + one) % (p - one);
        new_v = b;
    }

    return { new_x, new_u, new_v };
}

// Расширенный алгоритм Евклида с модульным вычитанием
std::tuple<BN, BN, BN> extended_euclid(const BN& a, const BN& b, const BN& n) {
    BN zero = 0;
    if (b == zero) {
        return { a, BN(1), BN(0) };
    }
    BN d, x1, y1;
    std::tie(d, x1, y1) = extended_euclid(b, a % b, n);
    BN x = y1;
    std::cout << "вот щас зайдём в mod_sub и пизда программе\n";
    BN temp;
    temp = a / b;
    // std::cout << a << " " << b << " " << y1 << "\n";
    std::cout << temp << "\n";
    temp = temp * y1;
    
    BN y = mod_sub(x1, temp, n);  // Используем mod_sub для безопасного вычитания
    
    return { d, x, y };
}

// Решение уравнения вида r*x ≡ (a2 - a1) mod n
BN solve_mod_equation(const BN& r, const BN& a1, const BN& a2, const BN& n) {
    BN zero = 0;
    BN a = mod_sub(a2, a1, n);  // Используем mod_sub для корректного вычитания
    BN d, x, y;
    std::cout << "теперь щас зайдём в extended_euclid\n";
    std::tie(d, x, y) = extended_euclid(r, n, n);  // Передаём n как модуль

    if (a % d != zero) {
        throw std::invalid_argument("No solutions");
    }

    BN z0 = (x * (a / d)) % n;
    return (z0 + n) % n;
}

// Основной алгоритм Полларда (ρ-метод)
BN pollard_rho(const BN& g, const BN& p, const BN& n, const BN& a) {
    BN x1 = 1, x2 = 1;
    BN y1 = 0, y2 = 0;
    BN b1 = 0, b2 = 0;
    BN zero = 0, one = 1;

    // Вывод заголовка таблицы
    std::cout << "x1\ty1\tb1\tx2\ty2\tb2\n";
    std::cout << "------------------------------------------\n";
    std::cout << x1 << "\t" << y1 << "\t" << b1 << "\t" << x2 << "\t" << y2 << "\t" << b2 << "\n";

    while (true) {
        std::tie(x1, y1, b1) = F(x1, y1, b1, g, a, p);
        std::tie(x2, y2, b2) = F(x2, y2, b2, g, a, p);
        std::tie(x2, y2, b2) = F(x2, y2, b2, g, a, p);

        // Вывод значений на каждой итерации
        std::cout << x1 << "\t" << y1 << "\t" << b1 << "\t" << x2 << "\t" << y2 << "\t" << b2 << "\n";

        if (x1 == x2) {
            BN r = mod_sub(b1, b2, n);
            if (r == zero) {
                std::cerr << "Solution not found. CRITICAL STOP." << std::endl;
                return BN(-1);
            }

            std::cout << "щас зайдём в solve_mod_equation\n";
            BN result = solve_mod_equation(r, y1, y2, n);
            BN d = r.gcd(n);

            BN i = zero;
            while (i < d) {
                BN z = (result + i * (n / d)) % n;
                if (g.pow_mod(z, p) == a) {
                    return z;
                }
                i = i + one;
            }

            std::cerr << "Solution not found after testing all possible z." << std::endl;
            return BN(-1);
        }
    }
}

int main() {
    BN g = 2;
    BN p = 29;
    BN a = 5;
    BN n = 28;

    try {
        BN result = pollard_rho(g, p, n, a);
        std::cout << "log_" << g << "(" << a << ") mod " << p << " = " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return 0;
}
