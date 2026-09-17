#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    char ch;
    char *word = NULL;
    char *result = NULL;
    int i, j, pos = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        if (rank == 0)
            printf("Please run the program with exactly N processes.\n");

        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        word = malloc((size + 1) * sizeof(char));

        printf("Enter a word of length %d: ", size);
        scanf("%s", word);
    }

    MPI_Scatter(word, 1, MPI_CHAR,
                &ch, 1, MPI_CHAR,
                0, MPI_COMM_WORLD);

    int count = rank + 1;
    char *local = malloc((count + 1) * sizeof(char));

    for (i = 0; i < count; i++)
        local[i] = ch;

    local[count] = '\0';

    int *counts = NULL;
    int *displacements = NULL;

    if (rank == 0)
    {
        counts = malloc(size * sizeof(int));
        displacements = malloc(size * sizeof(int));

        for (i = 0; i < size; i++)
        {
            counts[i] = i + 1;
            displacements[i] = i * (i + 1) / 2;
        }

        int total = size * (size + 1) / 2;
        result = malloc((total + 1) * sizeof(char));
    }

    MPI_Gatherv(local, count, MPI_CHAR,
                result, counts, displacements, MPI_CHAR,
                0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total = size * (size + 1) / 2;
        result[total] = '\0';

        printf("Output : %s\n", result);

        free(word);
        free(result);
        free(counts);
        free(displacements);
    }

    free(local);

    MPI_Finalize();

    return 0;
}