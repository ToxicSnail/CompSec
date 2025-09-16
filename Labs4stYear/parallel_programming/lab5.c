#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define EPS 1e-5 
#define A 1.0    

double f(double x, double y) 
{
    return log(1.0 + x*x + y*y);
}

double integrate_x(double y, int M) 
{
    double xmax = sqrt(A*A - y*y); 
    double x_min = -xmax;
    double x_max =  xmax;
    double h = (x_max - x_min) / M; 

    double sum = f(x_min, y) + f(x_max, y);
    for (int i = 1; i < M; i++) 
    {
        double x = x_min + i * h;
        if (i % 2 == 0) 
        {
            sum += 2.0 * f(x, y);
        } else 
        {
            sum += 4.0 * f(x, y);
        }
    }
    sum *= h / 3.0;
    return sum;
}


double parallel_integral(int N, int M, int rank, int size) 
{
    double y_min = 0.0, y_max = A;
    double h = (y_max - y_min) / N;
    double local_sum = 0.0;

    for (int i = rank; i <= N; i += size) 
    {
        double y = y_min + i * h;
        double inner = integrate_x(y, M);


        double coeff;
        if (i == 0 || i == N)
        {
            coeff = 1.0;
        } else if (i % 2 == 0) 
        {
            coeff = 2.0;
        } else 
        {
            coeff = 4.0;
        }
        local_sum += coeff * inner;
    }
    return local_sum;
}

int main(int argc, char *argv[]) 
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 4;
    int M = 4;

    double integral_old = 0.0, integral_new = 0.0;
    double err = 0.0;
    int converged = 0;
    int iter = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    do 
    {
        double local_sum = parallel_integral(N, M, rank, size);
        MPI_Reduce(&local_sum, &integral_new, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) 
        {
            double h = (A - 0.0) / N; 
            integral_new *= (h / 3.0);
            
            err = fabs(integral_new - integral_old);
            //printf("Iteration %d: N=%d, M=%d, Integral = %.10lf, err = %.10lf\n", iter, N, M, integral_new, err);

            if (iter > 0 && err < EPS) 
            {
                converged = 1;
            }
            integral_old = integral_new;
        }

        MPI_Bcast(&converged, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (!converged) 
        {
            N *= 2;
            M *= 2;
            iter++;
        }
    } 
    while (!converged);

    double t_end = MPI_Wtime();
    if (rank == 0) 
    {
        double parallel_time = t_end - t_start;
        printf("Final integral value = %.10lf\n", integral_new);
        printf("Parallel computation time = %.6lf sec\n", parallel_time);
    }

    MPI_Finalize();
    return 0;
}