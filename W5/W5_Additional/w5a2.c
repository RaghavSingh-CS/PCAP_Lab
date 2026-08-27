#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, N, chunk;
    int A[100], local[100], result[100];
    int local_even = 0, local_odd = 0;
    int total_even = 0, total_odd = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter N: ");
        scanf("%d", &N);

        printf("Enter %d elements: ", N);
        for(int i = 0; i < N; i++)
            scanf("%d", &A[i]);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = N / size;

    MPI_Scatter(A, chunk, MPI_INT,
                local, chunk, MPI_INT,
                0, MPI_COMM_WORLD);

    for(int i = 0; i < chunk; i++)
    {
        if(local[i] % 2 == 0)
        {
            local[i] = 1;
            local_even++;
        }
        else
        {
            local[i] = 0;
            local_odd++;
        }
    }

    MPI_Gather(local, chunk, MPI_INT,
               result, chunk, MPI_INT,
               0, MPI_COMM_WORLD);

    MPI_Reduce(&local_even, &total_even, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Reduce(&local_odd, &total_odd, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        printf("Resultant Array: ");

        for(int i = 0; i < N; i++)
            printf("%d ", result[i]);

        printf("\nEven Count = %d\n", total_even);
        printf("Odd Count = %d\n", total_odd);
    }

    MPI_Finalize();
    return 0;
}