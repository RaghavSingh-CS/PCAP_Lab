#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int buffer_size = size * (sizeof(int) + MPI_BSEND_OVERHEAD);
    void *buffer = malloc(buffer_size);

    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0)
    {
        int arr[size];

        printf("Enter %d elements:\n", size);

        for (int i = 0; i < size; i++)
            scanf("%d", &arr[i]);

        // Root keeps its own element
        value = arr[0];

        // Send one element to every other process
        for (int i = 1; i < size; i++)
        {
            MPI_Bsend(&arr[i], 1, MPI_INT,
                      i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&value, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    if (rank % 2 == 0)
    {
        printf("Process %d received %d, square = %d\n",
               rank, value, value * value);
    }
    else
    {
        printf("Process %d received %d, cube = %d\n",
               rank, value, value * value * value);
    }

    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    MPI_Finalize();

    return 0;
}