#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    char str[] = "PAraSiTe";

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < strlen(str))
    {
        char ch = str[rank];

        if (islower(ch))
            ch = toupper(ch);
        else if (isupper(ch))
            ch = tolower(ch);

        printf("Process %d toggled %c to %c\n",
               rank, str[rank], ch);
    }

    MPI_Finalize();
    return 0;
}