#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TOTAL_POINTS 10000000

int main() {
    int thread_counts[] = {1, 2, 4, 8};

    for (int t = 0; t < 4; t++) {
        int threads = thread_counts[t];
        omp_set_num_threads(threads);

        int count_inside = 0;
        double start = omp_get_wtime();

        #pragma omp parallel reduction(+:count_inside)
        {
            // Seed distinct state per thread
            unsigned int seed = 12345 + omp_get_thread_num();

            #pragma omp for
            for (int i = 0; i < TOTAL_POINTS; i++) {
                double x = (double)rand_r(&seed) / RAND_MAX;
                double y = (double)rand_r(&seed) / RAND_MAX;

                if (x * x + y * y <= 1.0) {
                    count_inside++;
                }
            }
        }

        double time_taken = omp_get_wtime() - start;
        double pi_est = 4.0 * count_inside / TOTAL_POINTS;

        printf("Threads: %d | Pi: %.5f | Time: %f s\n", threads, pi_est, time_taken);
    }

    return 0;
}