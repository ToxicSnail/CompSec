#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N = 10; // Размерность матрицы A (NxN) и вектора x
    int local_rows; // Количество строк матрицы для каждого процесса
    int *A_global = NULL, *x_global = NULL, *y_global = NULL;
    int *A_local = NULL, *x_local = NULL, *y_local = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Убедимся, что размерность матрицы делится на количество процессов
    if (N % size != 0) 
    {
        if (rank == 0) 
        {
            printf("Error: Matrix size N must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    local_rows = N / size;

    // Инициализация глобальных данных на процессе 0
    if (rank == 0) 
    {
        A_global = (int *)malloc(N * N * sizeof(int));
        x_global = (int *)malloc(N * sizeof(int));
        y_global = (int *)malloc(N * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < N * N; i++) 
        {
            A_global[i] = rand() % 10; // Заполнение матрицы случайными числами
        }
        for (int i = 0; i < N; i++) 
        {
            x_global[i] = rand() % 10; // Заполнение вектора случайными числами
        }
    }

    // Выделение памяти для локальных данных
    A_local = (int *)malloc(local_rows * N * sizeof(int));
    x_local = (int *)malloc(N * sizeof(int));
    y_local = (int *)malloc(local_rows * sizeof(int));

    // Шаг 1: Разослать вектор x на все процессы с помощью MPI_Bcast
    if (rank == 0) 
    {
        MPI_Bcast(x_global, N, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else 
    {
        MPI_Bcast(x_local, N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Шаг 2: Распределить строки матрицы A по процессам с помощью MPI_Scatter
    if (rank == 0) 
    {
        MPI_Scatter(A_global, local_rows * N, MPI_INT, A_local, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else 
    {
        MPI_Scatter(MPI_IN_PLACE, local_rows * N, MPI_INT, A_local, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Шаг 3: Локальное умножение матрицы на вектор
    for (int i = 0; i < local_rows; i++) 
    {
        y_local[i] = 0;
        for (int j = 0; j < N; j++) 
        {
            y_local[i] += A_local[i * N + j] * x_local[j];
        }
    }

    // Шаг 4: Собрать результаты в полный вектор y с помощью MPI_Allgather
    if (rank == 0) 
    {
        y_global = (int *)malloc(N * sizeof(int));
    }
    MPI_Allgather(y_local, local_rows, MPI_INT, y_global, local_rows, MPI_INT, MPI_COMM_WORLD);

    // Проверка правильности результата на процессе 0
    if (rank == 0) 
    {
        int *sequential_y = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) 
        {
            sequential_y[i] = 0;
            for (int j = 0; j < N; j++) 
            {
                sequential_y[i] += A_global[i * N + j] * x_global[j];
            }
        }

        // Сравнение параллельного и последовательного результатов
        int is_correct = 1;
        for (int i = 0; i < N; i++) 
        {
            if (y_global[i] != sequential_y[i]) 
            {
                is_correct = 0;
                break;
            }
        }

        if (is_correct) 
        {
            printf("Result is correct!\n");
        } else 
        {
            printf("Result is incorrect.\n");
        }

        // Вывод результатов
        printf("Parallel result (y):\n");
        for (int i = 0; i < N; i++) 
        {
            printf("%d ", y_global[i]);
        }
        printf("\n");

        free(A_global);
        free(x_global);
        free(y_global);
        free(sequential_y);
    }

    free(A_local);
    free(x_local);
    free(y_local);

    MPI_Finalize();
    return 0;
}