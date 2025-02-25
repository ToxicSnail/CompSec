#include <stdio.h>
#include <math.h>

int main() {
    double x, epsilon;
    double left_part, right_part, term, difference;
    int iteration_count = 0;

    printf("Enter x (|x| < 1): ");
    scanf("%lf", &x);

    if (fabs(x) >= 1) {
        printf("Error: |x| must be < 1.\n");
        return 1;
    }

    printf("Enter epsilon: ");
    scanf("%lf", &epsilon);

    if (epsilon <= 0) {
        printf("Error: epsilon must be > 0.\n");
        return 1;
    }

    // Вычисление точного значения с помощью стандартной функции
    left_part = log(1 + x);

    // Итерационный расчет через разложение в ряд
    term = x; // Первый член ряда
    right_part = term; // Начальная правая часть
    difference = fabs(left_part - right_part); // Начальная разница

    printf("\nInitial left_part: %.10lf\n", left_part);
    printf("Initial right_part: %.10lf\n", right_part);
    printf("Initial Difference: %.10lf\n\n", difference);

    while (difference >= epsilon) {
        iteration_count++;
        // Добавляем следующий член ряда
        term *= -x * iteration_count / (iteration_count + 1); // Следующий член
        right_part += term;

        // Пересчитываем разницу
        difference = fabs(left_part - right_part);

        // Печать промежуточных значений
        printf("Iteration %d: Left part = %.10lf, Right part = %.10lf, Difference = %.10lf\n",
               iteration_count, left_part, right_part, difference);
    }

    // Итог
    printf("\nFinal result:\n");
    printf("Left part (log(1 + x)): %.10lf\n", left_part);
    printf("Right part (series expansion): %.10lf\n", right_part);
    printf("Iterations: %d\n", iteration_count);

    return 0;
}
