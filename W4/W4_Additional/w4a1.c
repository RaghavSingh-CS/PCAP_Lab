#include <stdio.h>
#include <mpi.h>

int reverseNumber(int n)
{
    int rev = 0;

    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }

    return rev;
}

int main(int argc, char *argv[])
{
    int rank, size;

    int arr[9] = {18, 523, 301, 1234, 2, 14, 108, 150, 1928};

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < 9)
    {
        int result = reverseNumber(arr[rank]);

        printf("Process %d: %d -> %d\n",
               rank, arr[rank], result);
    }

    MPI_Finalize();

    return 0;
}