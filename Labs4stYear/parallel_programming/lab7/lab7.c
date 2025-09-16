#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

void serial_mul(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
}

void parallel_ikj(double *A, double *B, double *C, int n) {
    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
}

void parallel_kij(double *A, double *B, double *C, int n) {
    #pragma omp parallel for schedule(runtime)
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
}

// Вспомогательные функции для отображения схемы из OMP_SCHEDULE
const char* sched_name(omp_sched_t kind) {
    switch (kind) {
        case omp_sched_static:  return "static";
        case omp_sched_dynamic: return "dynamic";
        case omp_sched_guided:  return "guided";
        case omp_sched_auto:    return "auto";
        default:                return "unknown";
    }
}

int main(int argc, char *argv[]) {
    int n    = (argc > 1 ? atoi(argv[1]) : 2048);
    int reps = (argc > 2 ? atoi(argv[2]) : 3);

    int threads = omp_get_max_threads();
    omp_sched_t kind;
    int chunk;
    omp_get_schedule(&kind, &chunk);

    printf("Config: n=%d, threads=%d, schedule=%s, chunk=%d, reps=%d\n",
           n, threads, sched_name(kind), chunk, reps);

    srand48(time(NULL));
    size_t sz = (size_t)n * n;

    double *A     = malloc(sz * sizeof(double));
    double *B     = malloc(sz * sizeof(double));
    double *C_ser = malloc(sz * sizeof(double));
    double *C_ikj = malloc(sz * sizeof(double));
    double *C_kij = malloc(sz * sizeof(double));
    if (!A || !B || !C_ser || !C_ikj || !C_kij) {
        fprintf(stderr, "Memory allocation error\n");
        return EXIT_FAILURE;
    }

    // Инициализация A и B случайными числами
    for (size_t i = 0; i < sz; i++) {
        A[i] = drand48();
        B[i] = drand48();
    }

    double t, t_sum;
    double T_ser, T_ikj, T_kij;
    double err_ikj = 0, err_kij = 0;

    // --- Последовательный ---
    t_sum = 0;
    for (int r = 0; r < reps; r++) {
        memset(C_ser, 0, sz * sizeof(double));
        t = omp_get_wtime();
        serial_mul(A, B, C_ser, n);
        t_sum += omp_get_wtime() - t;
    }
    T_ser = t_sum / reps;

    // --- Параллельный i-k-j ---
    t_sum = 0;
    for (int r = 0; r < reps; r++) {
        memset(C_ikj, 0, sz * sizeof(double));
        t = omp_get_wtime();
        parallel_ikj(A, B, C_ikj, n);
        t_sum += omp_get_wtime() - t;
    }
    T_ikj = t_sum / reps;

    t_sum = 0;
    for (int r = 0; r < reps; r++) {
        memset(C_kij, 0, sz * sizeof(double));
        t = omp_get_wtime();
        parallel_kij(A, B, C_kij, n);
        t_sum += omp_get_wtime() - t;
    }
    T_kij = t_sum / reps;

    for (size_t i = 0; i < sz; i++) {
        err_ikj = fmax(err_ikj, fabs(C_ser[i] - C_ikj[i]));
        err_kij = fmax(err_kij, fabs(C_ser[i] - C_kij[i]));
    }

    printf("T_ser=%.6f\n", T_ser);
    printf("T_ikj=%.6f\n", T_ikj);
    printf("T_kij=%.6f\n", T_kij);
    printf("Err_ikj=%e\n", err_ikj);
    printf("Err_kij=%e\n", err_kij);

    free(A);
    free(B);
    free(C_ser);
    free(C_ikj);
    free(C_kij);
    return 0;
}
