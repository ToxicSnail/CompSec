#include <stdio.h>
#include <math.h>

int main() {
    double x_start = 0.1, x_end = 1.15, h = 0.05; // Начальное значение, конечное значение, шаг
    double x, y;

    // Заголовок таблицы
    printf("x y\n");

    // Цикл для вычисления значений функции
    for (x = x_start; x <= x_end; x += h) {
        // Вычисление y = x^2 * e^(1/x)
        double root_e = pow(exp(1), 1.0 / x); // \(\sqrt[x]{e} = e^{1/x}\)
        y = x * x * root_e;

        // Вывод значений
        printf("%.2lf %.6lf\n", x, y);
    }

    return 0;
}
