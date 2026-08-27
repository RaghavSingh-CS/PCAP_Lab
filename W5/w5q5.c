#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void selectionSort(int a[], int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        int min = i;

        for(int j = i + 1; j < n; j++)
            if(a[j] < a[min])
                min = j;

        int temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

void merge(int a[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for(int i = 0; i < n1; i++)
        L[i] = a[left + i];

    for(int i = 0; i < n2; i++)
        R[i] = a[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while(i < n1 && j < n2)
    {
        if(L[i] <= R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }

    while(i < n1)
        a[k++] = L[i++];

    while(j < n2)
        a[k++] = R[j++];
}

int main(int argc, char *argv[])
{
    int rank, size, n, chunk;
    int a[100], local[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter %d elements: ", n);

        for(int i = 0; i < n; i++)
            scanf("%d", &a[i]);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    chunk = n / size;

    MPI_Scatter(a, chunk, MPI_INT,
                local, chunk, MPI_INT,
                0, MPI_COMM_WORLD);

    selectionSort(local, chunk);

    MPI_Gather(local, chunk, MPI_INT,
               a, chunk, MPI_INT,
               0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        int currentSize = chunk;

        while(currentSize < n)
        {
            for(int left = 0; left < n; left += 2 * currentSize)
            {
                int mid = left + currentSize - 1;
                int right = left + 2 * currentSize - 1;

                if(right >= n)
                    right = n - 1;

                if(mid < n - 1)
                    merge(a, left, mid, right);
            }

            currentSize *= 2;
        }

        printf("Sorted array: ");

        for(int i = 0; i < n; i++)
            printf("%d ", a[i]);

        printf("\n");
    }

    MPI_Finalize();
    return 0;
}