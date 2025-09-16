#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N = 4;
    int local_rows;
    int *A_global = NULL, *x_global = NULL, *y_global = NULL;
    int *A_local = NULL, *y_local = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Error: Matrix size N must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    local_rows = N / size;

    if (rank == 0) {
        A_global = (int *)malloc(N * N * sizeof(int));
        x_global = (int *)malloc(N * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < N * N; i++) {
            A_global[i] = rand() % 10;
        }
        for (int i = 0; i < N; i++) {
            x_global[i] = rand() % 10;
        }
    }

    if (rank != 0) {
        x_global = (int *)malloc(N * sizeof(int));
    }

    MPI_Bcast(x_global, N, MPI_INT, 0, MPI_COMM_WORLD);

    A_local = (int *)malloc(local_rows * N * sizeof(int));
    y_local = (int *)malloc(local_rows * sizeof(int));
    y_global = (int *)malloc(N * sizeof(int));

    MPI_Scatter(A_global, local_rows * N, MPI_INT, A_local, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        y_local[i] = 0;
        for (int j = 0; j < N; j++) {
            y_local[i] += A_local[i * N + j] * x_global[j];
        }
    }

    MPI_Allgather(y_local, local_rows, MPI_INT, y_global, local_rows, MPI_INT, MPI_COMM_WORLD);

    printf("Process %d: Local result (y_local):\n", rank);
    for (int i = 0; i < local_rows; i++) {
        printf("%d ", y_local[i]);
    }
    printf("\n");

    printf("Process %d: Global result (y_global):\n", rank);
    for (int i = 0; i < N; i++) {
        printf("%d ", y_global[i]);
    }
    printf("\n");

    if (rank == 0) {
        int *sequential_y = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            sequential_y[i] = 0;
            for (int j = 0; j < N; j++) {
                sequential_y[i] += A_global[i * N + j] * x_global[j];
            }
        }

        int is_correct = 1;
        for (int i = 0; i < N; i++) {
            if (y_global[i] != sequential_y[i]) {
                is_correct = 0;
                break;
            }
        }

        if (is_correct) {
            printf("Result is correct!\n");
        } else {
            printf("Result is incorrect.\n");
        }

        free(A_global);
        free(x_global);
        free(sequential_y);
    }

    free(A_local);
    free(y_local);
    free(y_global);

    MPI_Finalize();
    return 0;
}