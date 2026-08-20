#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int x = 2;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int result = (int)pow(x, rank);

    printf("Process %d of %d: %d^%d = %d\n",
           rank, size, x, rank, result);

    MPI_Finalize();

    return 0;
}