#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;

    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr[i] = 1;  
    }

    long long serial_sum = 0;
    long long parallel_sum = 0;

    double start, end;

    start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        serial_sum += arr[i];
    }

    end = omp_get_wtime();

    double serial_time = end - start;

    start = omp_get_wtime();

    #pragma omp parallel for reduction(+:parallel_sum)
    for (int i = 0; i < n; i++) {
        parallel_sum += arr[i];
    }

    end = omp_get_wtime();

    double parallel_time = end - start;

    printf("\nSerial Sum   = %lld\n", serial_sum);
    printf("Parallel Sum = %lld\n", parallel_sum);

    printf("\nSerial Execution Time   = %f seconds\n", serial_time);
    printf("Parallel Execution Time = %f seconds\n", parallel_time);

    printf("Speedup = %.2f\n", serial_time / parallel_time);

    free(arr);

    return 0;
}