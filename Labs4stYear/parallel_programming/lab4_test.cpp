#include <mpi.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>


double f(double x) 
{
    return std::log(1.0 + x) / x; //f(x) = ln(1+x)/x
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.1, b = 1.0;
    double epsilon = 1e-10;
    long long N = 1000000;
    if (argc > 1) 
    {
        N = std::atoll(argv[1]);
    }

    double result_old = 0.0, result_new = 0.0;
    bool converged = false;

    double time1 = 0.0, time2 = 0.0;
    if (rank == 0) 
    {
        time1 = MPI_Wtime();
    }

    while (!converged) 
    {
        long long local_N = N / size;

        // Определяем локальные границы интегрирования для данного rank
        double local_a = a + (b - a) * (rank / static_cast<double>(size));
        double local_b = local_a + (b - a) / size;

        // Считаем локальную сумму методом средних прямоугольников
        double local_sum = 0.0;
        for (long long i = 1; i <= local_N; i++) 
        {
            double x = local_a + (local_b - local_a) * ((i - 0.5) / local_N);   // Середина i-го «прямоугольника» внутри [local_a, local_b]
            local_sum += f(x);
        }

        double global_sum = 0.0;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) 
        {
            result_new = (b - a) / static_cast<double>(N) * global_sum;

            if (N != 1000000 && std::fabs(result_new - result_old) < epsilon) 
            {
                converged = true;
            }
            result_old = result_new;
            N *= 2;
        }

        MPI_Bcast(&converged, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
        MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) 
    {
        time2 = MPI_Wtime();
        std::cout << std::fixed << std::setprecision(10) << "Result = " << result_new << ", Time = " << (time2 - time1) << " sec" << std::endl;
    }

    MPI_Finalize();
    return 0;
}




// local_size * size = (N / size) * size = N;

// pguii24c