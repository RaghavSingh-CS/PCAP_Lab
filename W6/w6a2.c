#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int A[5][5];
    int local_row[5];
    int all_rows[5][5];
    int local_result[5];
    int B[5][5];
    int i, j;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 5)
    {
        if (rank == 0)
            printf("Please run the program with exactly 5 processes.\n");

        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        printf("Enter 5 x 5 matrix A:\n");

        for (i = 0; i < 5; i++)
            for (j = 0; j < 5; j++)
                scanf("%d", &A[i][j]);
    }

    MPI_Scatter(A, 5, MPI_INT,
                local_row, 5, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Allgather(local_row, 5, MPI_INT,
                  all_rows, 5, MPI_INT,
                  MPI_COMM_WORLD);

    int column_min = all_rows[0][rank];
    int column_max = all_rows[0][rank];

    for (i = 1; i < 5; i++)
    {
        if (all_rows[i][rank] < column_min)
            column_min = all_rows[i][rank];

        if (all_rows[i][rank] > column_max)
            column_max = all_rows[i][rank];
    }

    for (j = 0; j < 5; j++)
    {
        if (j == rank)
            local_result[j] = 0;
        else if (j < rank)
            local_result[j] = column_max;
        else
            local_result[j] = column_min;
    }

    MPI_Gather(local_result, 5, MPI_INT,
               B, 5, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nMatrix B:\n");

        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
                printf("%d ", B[i][j]);

            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}