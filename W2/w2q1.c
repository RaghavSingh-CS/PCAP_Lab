#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int m, n;

    printf("Enter number of rows and columns: ");
    scanf("%d %d", &m, &n);

    int **a = malloc(m * sizeof(int *));
    int **b = malloc(m * sizeof(int *));
    int **serial_c = malloc(m * sizeof(int *));
    int **parallel_c = malloc(m * sizeof(int *));

    for (int i = 0; i < m; i++) {
        a[i] = malloc(n * sizeof(int));
        b[i] = malloc(n * sizeof(int));
        serial_c[i] = malloc(n * sizeof(int));
        parallel_c[i] = malloc(n * sizeof(int));
    }

    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    printf("Enter elements of matrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &b[i][j]);
        }
    }

    double serial_start = omp_get_wtime();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            serial_c[i][j] = a[i][j] + b[i][j];
        }
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        int thread_id = omp_get_thread_num();

        for (int j = 0; j < n; j++) {
            parallel_c[i][j] = a[i][j] + b[i][j];
        }

        printf("Thread %d computed row %d\n", thread_id, i);
    }

    double parallel_end = omp_get_wtime();

    double serial_time = serial_end - serial_start;
    double parallel_time = parallel_end - parallel_start;

    int num_threads = 1;

    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    double speedup = serial_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("\nResultant Matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", parallel_c[i][j]);
        }
        printf("\n");
    }

    int correct = 1;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (serial_c[i][j] != parallel_c[i][j]) {
                correct = 0;
                break;
            }
        }
    }

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

    for (int i = 0; i < m; i++) {
        free(a[i]);
        free(b[i]);
        free(serial_c[i]);
        free(parallel_c[i]);
    }

    free(a);
    free(b);
    free(serial_c);
    free(parallel_c);

    return 0;
}