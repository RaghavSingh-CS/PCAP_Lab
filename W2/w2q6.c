#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;

    printf("Enter size of vectors: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    int *serial_add = malloc(n * sizeof(int));
    int *serial_sub = malloc(n * sizeof(int));
    int *serial_mul = malloc(n * sizeof(int));

    int *parallel_add = malloc(n * sizeof(int));
    int *parallel_sub = malloc(n * sizeof(int));
    int *parallel_mul = malloc(n * sizeof(int));

    printf("Enter elements of vector A:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    printf("Enter elements of vector B:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &b[i]);
    }

    double serial_start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        serial_add[i] = a[i] + b[i];
    }

    for (int i = 0; i < n; i++) {
        serial_sub[i] = a[i] - b[i];
    }

    for (int i = 0; i < n; i++) {
        serial_mul[i] = a[i] * b[i];
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Thread %d performing vector addition\n",
                   omp_get_thread_num());

            for (int i = 0; i < n; i++) {
                parallel_add[i] = a[i] + b[i];
            }
        }

        #pragma omp section
        {
            printf("Thread %d performing vector subtraction\n",
                   omp_get_thread_num());

            for (int i = 0; i < n; i++) {
                parallel_sub[i] = a[i] - b[i];
            }
        }

        #pragma omp section
        {
            printf("Thread %d performing vector multiplication\n",
                   omp_get_thread_num());

            for (int i = 0; i < n; i++) {
                parallel_mul[i] = a[i] * b[i];
            }
        }
    }

    double parallel_end = omp_get_wtime();

    int num_threads = 1;

    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
        }
    }

    int correct = 1;

    for (int i = 0; i < n; i++) {
        if (serial_add[i] != parallel_add[i] ||
            serial_sub[i] != parallel_sub[i] ||
            serial_mul[i] != parallel_mul[i]) {
            correct = 0;
            break;
        }
    }

    double serial_time = serial_end - serial_start;
    double parallel_time = parallel_end - parallel_start;
    double speedup = serial_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("\nAddition:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", parallel_add[i]);
    }

    printf("\n\nSubtraction:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", parallel_sub[i]);
    }

    printf("\n\nMultiplication:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", parallel_mul[i]);
    }

    printf("\n\nNumber of threads       = %d\n", num_threads);
    printf("Serial execution time   = %.10f seconds\n", serial_time);
    printf("Parallel execution time = %.10f seconds\n", parallel_time);
    printf("Speedup                  = %.4f\n", speedup);
    printf("Efficiency               = %.4f\n", efficiency);
    printf("Efficiency percentage    = %.2f%%\n", efficiency * 100);

    if (correct) {
        printf("Serial and parallel results match.\n");
    } else {
        printf("Serial and parallel results do not match.\n");
    }

    free(a);
    free(b);

    free(serial_add);
    free(serial_sub);
    free(serial_mul);

    free(parallel_add);
    free(parallel_sub);
    free(parallel_mul);

    return 0;
}