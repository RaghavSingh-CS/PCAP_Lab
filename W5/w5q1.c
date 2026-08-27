#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int num, factorial = 1;
    int a[100], factorials[100];
    int sum = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter %d values: ", size);

        for(int i = 0; i < size; i++)
            scanf("%d", &a[i]);
    }

    MPI_Scatter(a, 1, MPI_INT,
                &num, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    for(int i = 1; i <= num; i++)
        factorial *= i;

    printf("Process %d received %d and factorial = %d\n",
           rank, num, factorial);

    MPI_Gather(&factorial, 1, MPI_INT,
               factorials, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(int i = 0; i < size; i++)
            sum += factorials[i];

        printf("Sum of factorials = %d\n", sum);
    }

    MPI_Finalize();

    return 0;
}