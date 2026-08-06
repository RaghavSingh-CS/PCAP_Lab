#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int m, n;

    printf("Enter rows and columns of matrix: ");
    scanf("%d %d", &m, &n);

    int **A = (int **)malloc(m * sizeof(int *));
    int *x = (int *)malloc(n * sizeof(int));
    int *serial_y = (int *)malloc(m * sizeof(int));
    int *parallel_y = (int *)malloc(m * sizeof(int));

    for (int i = 0; i < m; i++)
        A[i] = (int *)malloc(n * sizeof(int));

    printf("Enter matrix A:\n");
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter vector:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);

    double serial_start = omp_get_wtime();

    for (int i = 0; i < m; i++) {
        serial_y[i] = 0;
        for (int j = 0; j < n; j++)
            serial_y[i] += A[i][j] * x[j];
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        parallel_y[i] = 0;

        for (int j = 0; j < n; j++)
            parallel_y[i] += A[i][j] * x[j];

        printf("Thread %d processed row %d\n",
               omp_get_thread_num(), i);
    }

    double parallel_end = omp_get_wtime();

    int num_threads = 1;

    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    int correct = 1;

    for (int i = 0; i < m; i++) {
        if (serial_y[i] != parallel_y[i]) {
            correct = 0;
            break;
        }
    }

    double serial_time = serial_end - serial_start;
    double parallel_time = parallel_end - parallel_start;
    double speedup = serial_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("\nResult Vector:\n");

    for (int i = 0; i < m; i++)
        printf("%d ", parallel_y[i]);

    printf("\n");

    printf("\nNumber of threads       = %d\n", num_threads);
    printf("Serial execution time   = %.10f seconds\n", serial_time);
    printf("Parallel execution time = %.10f seconds\n", parallel_time);
    printf("Speedup                 = %.4f\n", speedup);
    printf("Efficiency              = %.4f\n", efficiency);
    printf("Efficiency Percentage   = %.2f%%\n", efficiency * 100);

    if (correct)
        printf("Serial and parallel results match.\n");
    else
        printf("Serial and parallel results do not match.\n");

    for (int i = 0; i < m; i++)
        free(A[i]);

    free(A);
    free(x);
    free(serial_y);
    free(parallel_y);

    return 0;
}