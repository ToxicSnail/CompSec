#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    double temp, max_temp = -INFINITY, min_temp = INFINITY;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);
    double random_value = rand() * 1.0 / RAND_MAX;
    temp = pow(cos(random_value), rank + 1);

    printf("Process %d: Generated temp = %.6f\n", rank, temp);

    if (rank == 0) 
    {
        for (int i = 1; i < size; i++) 
        {
            double received_temp;
            MPI_Recv(&received_temp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (received_temp > max_temp) 
                max_temp = received_temp;
            if (received_temp < min_temp) 
                min_temp = received_temp;
        }
        if (temp > max_temp) 
            max_temp = temp;
        if (temp < min_temp) 
            min_temp = temp;

        for (int i = 1; i < size; i++) 
        {
            MPI_Send(&max_temp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&min_temp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } 
    else 
    {
        MPI_Send(&temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(&max_temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&min_temp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank == 0) {
        printf("Maximum temp across all processes: %.6f\n", max_temp);
        printf("Minimum temp across all processes: %.6f\n", min_temp);
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