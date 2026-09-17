#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value, received;
    int *array = NULL;
    int i, phase;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        array = malloc(size * sizeof(int));

        printf("Enter %d elements:\n", size);

        for (i = 0; i < size; i++)
            scanf("%d", &array[i]);
    }

    MPI_Scatter(array, 1, MPI_INT,
                &value, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    for (phase = 0; phase < size; phase++)
    {
        int partner;

        if (phase % 2 == 0)
        {
            if (rank % 2 == 0)
                partner = rank + 1;
            else
                partner = rank - 1;
        }
        else
        {
            if (rank % 2 == 0)
                partner = rank - 1;
            else
                partner = rank + 1;
        }

        if (partner >= 0 && partner < size)
        {
            MPI_Sendrecv(&value, 1, MPI_INT, partner, 0,
                         &received, 1, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (rank < partner)
            {
                if (value > received)
                    value = received;
            }
            else
            {
                if (value < received)
                    value = received;
            }
        }
    }

    MPI_Gather(&value, 1, MPI_INT,
               array, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Sorted array:\n");

        for (i = 0; i < size; i++)
            printf("%d ", array[i]);

        printf("\n");

        free(array);
    }

    MPI_Finalize();

    return 0;
}