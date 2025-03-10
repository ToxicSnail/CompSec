//задание 13

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    int rank, size;
    int *data = NULL; 
    int *recvbuf = NULL; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    data = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) 
    {
        data[i] = rank + i;
    }

    printf("Process %d: Data = ", rank);
    for (int i = 0; i < size; i++) 
    {
        printf("%d ", data[i]);
    }
    printf("\n");

    recvbuf = (int *)malloc(sizeof(int));

    int recv_counts[size];
    for (int i = 0; i < size; i++) 
    {
        recv_counts[i] = 1; 
    }

    MPI_Reduce_scatter(data, recvbuf, recv_counts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d: Received sum = %d\n", rank, *recvbuf);

    free(data);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}