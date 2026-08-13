#include <stdio.h>
#include <string.h>
#include <omp.h>

int main() {
    char lines[6][100] = {
        "OpenMP is a powerful tool",
        "Data sharing requires careful consideration",
        "Parallel programming speeds up execution",
        "Synchronization constructs prevent race conditions",
        "Merge sort can be parallelized using tasks",
        "Histogram computation can use atomic operations"
    };
    int total_lines = 6;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Round-robin assignment
        for (int i = tid; i < total_lines; i += num_threads) {
            char line_copy[100];
            strcpy(line_copy, lines[i]);

            #pragma omp critical
            {
                printf("[Thread %d] processing line %d: \"%s\"\n", tid, i, lines[i]);
                char *token = strtok(line_copy, " ");
                while (token != NULL) {
                    printf("  -> Token: %s\n", token);
                    token = strtok(NULL, " ");
                }
            }
        }
    }
    return 0;
}