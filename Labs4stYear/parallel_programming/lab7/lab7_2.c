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
    #pragma omp parallel
    for (int k = 0; k < n; k++) {
        #pragma omp for schedule(runtime)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
    }
}

int main(int argc, char *argv[]) {
    int n = (argc > 1 ? atoi(argv[1]) : 2048);
    int reps = (argc > 2 ? atoi(argv[2]) : 3);
    int threads = omp_get_max_threads();
    int chunk;
    omp_sched_t sched;
    omp_get_schedule(&sched, &chunk);

    printf("Config: n=%d, threads=%d, schedule=dynamic, chunk=%d, reps=%d\n",
           n, threads, chunk, reps);

    srand48(time(NULL));
    size_t sz = (size_t)n * n;
    double *A = malloc(sz * sizeof(double));
    double *B = malloc(sz * sizeof(double));
    double *Cser = malloc(sz * sizeof(double));
    double *Cikj = malloc(sz * sizeof(double));
    double *Ckij = malloc(sz * sizeof(double));
    if (!A || !B || !Cser || !Cikj || !Ckij) return fprintf(stderr, "MemError\n"), EXIT_FAILURE;

    for (size_t i = 0; i < sz; i++) {
        A[i] = drand48();
        B[i] = drand48();
    }

    double t, sum_ser = 0, sum_ikj = 0, sum_kij = 0;
    double Tser, Tikj, Tkij;
    double err_ikj = 0, err_kij = 0;

    for (int r = 0; r < reps; r++) {
        memset(Cser, 0, sz * sizeof(double));
        t = omp_get_wtime();
        serial_mul(A, B, Cser, n);
        sum_ser += omp_get_wtime() - t;

        memset(Cikj, 0, sz * sizeof(double));
        t = omp_get_wtime();
        parallel_ikj(A, B, Cikj, n);
        sum_ikj += omp_get_wtime() - t;

        memset(Ckij, 0, sz * sizeof(double));
        t = omp_get_wtime();
        parallel_kij(A, B, Ckij, n);
        sum_kij += omp_get_wtime() - t;
    }

    Tser = sum_ser / reps;
    Tikj = sum_ikj / reps;
    Tkij = sum_kij / reps;

    for (size_t i = 0; i < sz; i++) {
        err_ikj = fmax(err_ikj, fabs(Cser[i] - Cikj[i]));
        err_kij = fmax(err_kij, fabs(Cser[i] - Ckij[i]));
    }

    printf("T_ser=%.6f\nT_ikj=%.6f\nT_kij=%.6f\nErr_ikj=%.6e\nErr_kij=%.6e\n",
           Tser, Tikj, Tkij, err_ikj, err_kij);

    free(A);
    free(B);
    free(Cser);
    free(Cikj);
    free(Ckij);
    return 0;
}
