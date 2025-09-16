#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) 
{
    int rank, size;
    double temp, max_temp, min_temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);
    double random_value = rand() * 1.0 / RAND_MAX;
    temp = pow(cos(random_value), rank + 1);

    printf("Process %d: Generated temp = %.6f\n", rank, temp);

    MPI_Allreduce(&temp, &max_temp, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&temp, &min_temp, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) 
    {
        printf("\nMaximum temp across all processes: %.6f\n", max_temp);
        printf("Minimum temp across all processes: %.6f\n\n", min_temp);
    }

    if (rank % 2 == 0) 
    {
        temp = max_temp;
    } 
    else 
    {
        temp = min_temp;
    }

    printf("Process %d: Updated temp = %.6f\n", rank, temp);

    MPI_Finalize();
    return 0;
}

//на каждом процессе свое число