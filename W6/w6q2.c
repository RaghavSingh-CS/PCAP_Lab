#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int matrix[3][3];
    int row[3];
    int search, local_count = 0, total_count = 0;
    int i, j;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3)
    {
        if (rank == 0)
            printf("Please run the program with exactly 3 processes.\n");

        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        printf("Enter 3 x 3 matrix:\n");

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
                scanf("%d", &matrix[i][j]);
        }

        printf("Enter element to search: ");
        scanf("%d", &search);
    }

    MPI_Bcast(&search, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(matrix, 3, MPI_INT,
                row, 3, MPI_INT,
                0, MPI_COMM_WORLD);

    for (i = 0; i < 3; i++)
    {
        if (row[i] == search)
            local_count++;
    }

    MPI_Reduce(&local_count, &total_count, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Number of occurrences of %d = %d\n", search, total_count);

    MPI_Finalize();

    return 0;
}