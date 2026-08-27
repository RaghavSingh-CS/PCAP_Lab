#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int rank, size, len, chunk;
    char s1[100], s2[100];
    char local1[100], local2[100];
    char localResult[200], result[200];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter string S1: ");
        scanf("%s", s1);

        printf("Enter string S2: ");
        scanf("%s", s2);

        len = strlen(s1);
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = len / size;

    MPI_Scatter(s1, chunk, MPI_CHAR,
                local1, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    MPI_Scatter(s2, chunk, MPI_CHAR,
                local2, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    for(int i = 0; i < chunk; i++)
    {
        localResult[2 * i] = local1[i];
        localResult[2 * i + 1] = local2[i];
    }

    MPI_Gather(localResult, 2 * chunk, MPI_CHAR,
               result, 2 * chunk, MPI_CHAR,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        result[2 * len] = '\0';
        printf("Resultant String: %s\n", result);
    }

    MPI_Finalize();
    return 0;
}