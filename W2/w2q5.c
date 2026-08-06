#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int n;

    printf("Enter size of array: ");
    scanf("%d", &n);

    int *array = malloc(n * sizeof(int));
    int *serial_square = malloc(n * sizeof(int));
    int *parallel_square = malloc(n * sizeof(int));

    srand(time(NULL));

    #pragma omp parallel
    {
        #pragma omp single
        {
            printf("Thread %d is generating the array\n",
                   omp_get_thread_num());

            for (int i = 0; i < n; i++) {
                array[i] = rand() % 100;
            }
        }
    }

    printf("\nGenerated Array:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");

    double serial_start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        serial_square[i] = array[i] * array[i];
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        parallel_square[i] = array[i] * array[i];

        printf("Thread %d processed element %d\n",
               omp_get_thread_num(), i);
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
        if (serial_square[i] != parallel_square[i]) {
            correct = 0;
            break;
        }
    }

    double serial_time = serial_end - serial_start;
    double parallel_time = parallel_end - parallel_start;
    double speedup = serial_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("\nSquared Array:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", parallel_square[i]);
    }

    printf("\n");

    printf("\nNumber of threads       = %d\n", num_threads);
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

    free(array);
    free(serial_square);
    free(parallel_square);

    return 0;
}