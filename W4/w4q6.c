#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter an integer: ");
        scanf("%d", &value);

        value++;

        MPI_Send(&value, 1, MPI_INT,
                 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&value, 1, MPI_INT,
                 size - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Root received final value: %d\n", value);
    }
    else
    {
        MPI_Recv(&value, 1, MPI_INT,
                 rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        value++;

        int next = (rank + 1) % size;

        MPI_Send(&value, 1, MPI_INT,
                 next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}