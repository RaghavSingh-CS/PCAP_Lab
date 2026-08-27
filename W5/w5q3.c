#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int rank, size;
    char str[100], local[100];
    int len, chunk;
    int local_count = 0;
    int counts[100];
    int total = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter string: ");
        scanf("%s", str);

        len = strlen(str);
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = len / size;

    MPI_Scatter(str, chunk, MPI_CHAR,
                local, chunk, MPI_CHAR,
                0, MPI_COMM_WORLD);

    for(int i = 0; i < chunk; i++)
    {
        char c = tolower(local[i]);

        if(c != 'a' && c != 'e' && c != 'i' &&
           c != 'o' && c != 'u')
        {
            local_count++;
        }
    }

    MPI_Gather(&local_count, 1, MPI_INT,
               counts, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(int i = 0; i < size; i++)
        {
            printf("Process %d non-vowels = %d\n", i, counts[i]);
            total += counts[i];
        }

        printf("Total non-vowels = %d\n", total);
    }

    MPI_Finalize();
    return 0;
}