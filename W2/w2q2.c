#include <stdio.h>
#include <omp.h>

#define N 5

int main() {
    int A[N][N];
    int serial_B[N][N];
    int parallel_B[N][N];

    printf("Enter elements of the 5 x 5 matrix A:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    double serial_start = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        int row_min = A[i][0];
        int row_max = A[i][0];

        for (int j = 1; j < N; j++) {
            if (A[i][j] < row_min) {
                row_min = A[i][j];
            }

            if (A[i][j] > row_max) {
                row_max = A[i][j];
            }
        }

        for (int j = 0; j < N; j++) {
            if (i == j) {
                serial_B[i][j] = 0;
            } else if (i > j) {
                serial_B[i][j] = row_max;
            } else {
                serial_B[i][j] = row_min;
            }
        }
    }

    double serial_end = omp_get_wtime();

    double parallel_start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int row_min = A[i][0];
        int row_max = A[i][0];

        for (int j = 1; j < N; j++) {
            if (A[i][j] < row_min) {
                row_min = A[i][j];
            }

            if (A[i][j] > row_max) {
                row_max = A[i][j];
            }
        }

        for (int j = 0; j < N; j++) {
            if (i == j) {
                parallel_B[i][j] = 0;
            } else if (i > j) {
                parallel_B[i][j] = row_max;
            } else {
                parallel_B[i][j] = row_min;
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

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", parallel_B[i][j]);
        }
        printf("\n");
    }

    int correct = 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (serial_B[i][j] != parallel_B[i][j]) {
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

    return 0;
}