#include <stdio.h>
#include <mpi.h>
#include <math.h>

int a = 2, b = 5;

double f(double x)
{
    return x * x;
}

double wypisziloscTrapezow(int iloscTrapezow, int i)
{
    double h = (double)(b - a) / iloscTrapezow;
    double field = fabs(f(a + (i - 1) * h)) + fabs(f(a + i * h));
    return field / 2 * h;
}

double poleProstokatow(int liczbaProstokatow, int i)
{

    double h = (double)(b - a) / liczbaProstokatow;
    double field = fabs(f(a + (i - 1) * h) * h) + fabs(f(a + i * h) * h);
    return field;
}



int main(int argc, char **argv)
 {

    int iloscTrapezow, nP, i;
    int tag = 13;
    double poleTrapezu, sum = 0;
    double poleProstokatu, sum2 = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &nP);
    MPI_Comm_size(MPI_COMM_WORLD, &iloscTrapezow);

    if (nP == iloscTrapezow - 1)
    {
    	i = nP + 1;
        poleTrapezu = wypisziloscTrapezow(iloscTrapezow, i);
        poleProstokatu = poleProstokatow(iloscTrapezow, i - 1);
        sum += poleTrapezu;
        sum2 += poleProstokatu;
        printf("\nNumer procesu = %d\n", nP);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);

        MPI_Send(&sum, 1, MPI_DOUBLE, nP - 1, tag, MPI_COMM_WORLD);
        MPI_Send(&sum2, 1, MPI_DOUBLE, nP - 1, tag, MPI_COMM_WORLD);
    }

    if (nP > 0 && nP < iloscTrapezow - 1)
    {
        MPI_Recv(&sum, 1, MPI_DOUBLE, nP + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&sum2, 1, MPI_DOUBLE, nP + 1, tag, MPI_COMM_WORLD, &status);

        i = nP + 1;
        poleTrapezu = wypisziloscTrapezow(iloscTrapezow, i);
        poleProstokatu = poleProstokatow(iloscTrapezow, i - 1);
        sum += poleTrapezu;
        sum2 += poleProstokatu;
        printf("\nNumer procesu = %d\n", nP);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);

        MPI_Send(&sum, 1, MPI_DOUBLE, nP -1, tag, MPI_COMM_WORLD);
        MPI_Send(&sum2, 1, MPI_DOUBLE, nP - 1, tag, MPI_COMM_WORLD);

    }

    if (nP == 0)
    {
        MPI_Recv(&sum, 1, MPI_DOUBLE, nP + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&sum2, 1, MPI_DOUBLE, nP + 1, tag, MPI_COMM_WORLD, &status);

        i = nP + 1;
        poleTrapezu = wypisziloscTrapezow(iloscTrapezow, i);
        poleProstokatu = poleProstokatow(iloscTrapezow, i - 1);
        sum += poleTrapezu;
        sum2 += poleProstokatu;
        printf("\nNumer procesu = %d\n", nP);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);
    }

    MPI_Finalize();
    return 0;
}
