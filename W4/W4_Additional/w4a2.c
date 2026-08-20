#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int isPrime(int n)
{
    if (n < 2)
        return 0;

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        int arr[size];

        printf("Enter %d elements:\n", size);

        for (int i = 0; i < size; i++)
            scanf("%d", &arr[i]);

        value = arr[0];

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&arr[i], 1, MPI_INT,
                     i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&value, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    if (isPrime(value))
        printf("Process %d: %d is prime\n", rank, value);
    else
        printf("Process %d: %d is not prime\n", rank, value);

    MPI_Finalize();

    return 0;
}