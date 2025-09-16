#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10;
    if (argc > 1) 
    {
        n = atoi(argv[1]);
    }

    int chunk = n / size;
    int remainder = n % size;  // Остаток от деления

    int start = rank * chunk + 1 + (rank < remainder ? rank : remainder);
    int end = start + chunk - 1 + (rank < remainder);

    long long local_sum = 0;
    
    printf("Процесс %d: {", rank);
    for (int i = start; i <= end; i++) 
    {
        local_sum += i;
        printf("%d%s", i, (i == end) ? "" : ", ");
    }
    printf("}\n");

    // Модифицированное каскадное суммирование
    for (int step = size - 1; step > 0; step--) 
    {
        if (rank == step) 
        {
            MPI_Send(&local_sum, 1, MPI_LONG_LONG, step - 1, 0, MPI_COMM_WORLD);
        }
        else if (rank == step - 1) 
        {
            long long recv_val = 0;
            MPI_Recv(&recv_val, 1, MPI_LONG_LONG, step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += recv_val;
        }
    }

    if (rank == 0) 
    {
        printf("Сумма для n=%d: %lld\n", n, local_sum);
    }

    MPI_Finalize();
    return 0;
}
