#include <stdio.h>
#include <mpi.h>
#include <math.h>

double PI(int N)
{
    double sum = 0;
    double f;
    for (int n = 1; n <= N; n++)
    {
        f = pow(-1, n - 1) / (2 * n - 1);
        sum += f;
    }
    sum = sum * 4;
    
    return sum;
}


int main(int argc, char **argv)
 {
    int N, pN;
    int tag = 10;
    double pi;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pN);
    MPI_Comm_size(MPI_COMM_WORLD, &N);

    if (pN == 0)
    {
        pi = PI(pN + 1);
        printf("\nNumer procesu = %d\n", pN);
        printf("Przewidywana wartość PI: %f\n", pi);
        MPI_Send(&pi, 1, MPI_DOUBLE, pN + 1, tag, MPI_COMM_WORLD);
    }

    if (pN > 0 && pN < N - 1)
    {
        MPI_Recv(&pi, 1, MPI_DOUBLE, pN - 1, tag, MPI_COMM_WORLD, &status);

        printf("\nNumer procesu = %d\n", pN);
        printf("Przewidywana wartość PI: %f\n", pi);
        pi = PI(pN + 1);
        printf("Wartość PI: %f\n", pi);

        MPI_Send(&pi, 1, MPI_DOUBLE, pN + 1, tag, MPI_COMM_WORLD);
    }

    if (pN == N - 1)
    {
        MPI_Recv(&pi, 1, MPI_DOUBLE, pN - 1, tag, MPI_COMM_WORLD, &status);
        printf("\nNumer procesu = %d\n", pN);
        printf("Przewidywana wartość PI: %f\n", pi);
        pi = PI(pN + 1);
        printf("Wartość PI: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
