#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int matrix[4][4];
    int row[4];
    int result[4];
    int output[4][4];
    int i, j;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        if (rank == 0)
            printf("Please run the program with exactly 4 processes.\n");

        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        printf("Enter 4 x 4 matrix:\n");

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                scanf("%d", &matrix[i][j]);

        printf("I/p matrix:\n");

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                printf("%d ", matrix[i][j]);
            printf("\n");
        }
    }

    MPI_Scatter(matrix, 4, MPI_INT,
                row, 4, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Scan(row, result, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    MPI_Gather(result, 4, MPI_INT,
               output, 4, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nO/p matrix:\n");

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                printf("%d ", output[i][j]);

            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}