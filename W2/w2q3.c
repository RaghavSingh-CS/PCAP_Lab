#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int ones_complement(int number) {
    int mask = 0;
    int temp = number;

    if (number == 0) {
        return 1;
    }

    while (temp > 0) {
        mask = (mask << 1) | 1;
        temp = temp >> 1;
    }

    return number ^ mask;
}

void print_binary(int number) {
    if (number == 0) {
        printf("0");
        return;
    }

    int binary[32];
    int index = 0;

    while (number > 0) {
        binary[index++] = number % 2;
        number = number / 2;
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%d", binary[i]);
    }
}

int main() {
    int m, n;

    printf("Enter number of rows and columns: ");
    scanf("%d %d", &m, &n);

    int **A = malloc(m * sizeof(int *));
    int **serial_D = malloc(m * sizeof(int *));
    int **parallel_D = malloc(m * sizeof(int *));

    for (int i = 0; i < m; i++) {
        A[i] = malloc(n * sizeof(int));
        serial_D[i] = malloc(n * sizeof(int));
        parallel_D[i] = malloc(n * sizeof(int));
    }

    printf("Enter elements of Matrix A:\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    double serial_start = omp_get_wtime();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                serial_D[i][j] = A[i][j];
            } else {
                serial_D[i][j] = ones_complement(A[i][j]);
            }
        }
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                parallel_D[i][j] = A[i][j];
            } else {
                parallel_D[i][j] = ones_complement(A[i][j]);
            }
        }

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

    double serial_time = serial_end - serial_start;
    double parallel_time = parallel_end - parallel_start;
    double speedup = serial_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("\nMatrix A:\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B (1's complement in binary):\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                printf("%d\t", A[i][j]);
            } else {
                print_binary(parallel_D[i][j]);
                printf("\t");
            }
        }
        printf("\n");
    }

    printf("\nMatrix D (decimal equivalent):\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", parallel_D[i][j]);
        }
        printf("\n");
    }

    int correct = 1;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (serial_D[i][j] != parallel_D[i][j]) {
                correct = 0;
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
        free(A[i]);
        free(serial_D[i]);
        free(parallel_D[i]);
    }

    free(A);
    free(serial_D);
    free(parallel_D);

    return 0;
}