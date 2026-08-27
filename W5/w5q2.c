#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, M;
    int A[100], local[100];
    float avg, averages[100], total_avg = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter M: ");
        scanf("%d", &M);
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        printf("Enter %d elements: ", size * M);
        for(int i = 0; i < size * M; i++)
            scanf("%d", &A[i]);
    }

    MPI_Scatter(A, M, MPI_INT,
                local, M, MPI_INT,
                0, MPI_COMM_WORLD);

    int sum = 0;
    for(int i = 0; i < M; i++)
        sum += local[i];

    avg = (float)sum / M;

    MPI_Gather(&avg, 1, MPI_FLOAT,
               averages, 1, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(int i = 0; i < size; i++)
            total_avg += averages[i];

        total_avg /= size;

        printf("Total Average = %.2f\n", total_avg);
    }

    MPI_Finalize();
    return 0;
}