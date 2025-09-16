#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define EPS 1e-5
#define A   1.0

double f(double x, double y) {
    return log(1.0 + x*x + y*y);
}

double integrate_x(double y, int M) {
    double xmax  = sqrt(A*A - y*y);
    double x_min = -xmax, x_max = xmax;
    double h     = (x_max - x_min) / M;
    double sum   = f(x_min, y) + f(x_max, y);

    for (int i = 1; i < M; i++) {
        double x = x_min + i * h;
        sum += ((i & 1) ? 4.0 : 2.0) * f(x, y);
    }
    return sum * (h / 3.0);
}

double parallel_integral(int N, int M) {
    double y_min = 0.0, y_max = A, h = (y_max - y_min) / N;
    double local_sum = 0.0;

    #pragma omp parallel for reduction(+:local_sum) schedule(runtime)
    for (int i = 0; i <= N; i++) {
        double y = y_min + i * h;
        double inner = integrate_x(y, M);

        double coeff;
        if (i == 0 || i == N)        coeff = 1.0;
        else if ((i & 1) == 0)       coeff = 2.0;
        else                         coeff = 4.0;

        local_sum += coeff * inner;
    }
    return local_sum;
}

int main(int argc, char *argv[]) {
    int N = 4, M = 4;
    double integral_old = 0.0, integral_new = 0.0;
    double err = 0.0;
    int converged = 0, iter = 0;

    double t_start = omp_get_wtime();

    do {
        double sum = parallel_integral(N, M);
        integral_new = sum * ((A - 0.0) / N) / 3.0;

        err = fabs(integral_new - integral_old);
        if (iter > 0 && err < EPS) {
            converged = 1;
        }

        integral_old = integral_new;
        if (!converged) {
            N *= 2;
            M *= 2;
            iter++;
        }
    } while (!converged);

    double t_end = omp_get_wtime();
    printf("Final integral value     = %.10f\n", integral_new);
    printf("OpenMP computation time  = %.6f sec (threads = %d)\n",
           t_end - t_start,
           omp_get_max_threads());

    return 0;
}
