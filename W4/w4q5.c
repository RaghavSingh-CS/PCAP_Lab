#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int number;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter a number: ");
        scanf("%d", &number);

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&number, 1, MPI_INT,
                     i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&number, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Process %d received %d\n",
               rank, number);
    }

    MPI_Finalize();
    return 0;
}