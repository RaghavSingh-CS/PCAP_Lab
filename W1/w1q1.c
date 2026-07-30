#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Hello from Thread %d\n", omp_get_thread_num());

        #pragma omp single
        {
            printf("Total Number of threads utilized: %d\n",
                   omp_get_num_threads());
        }
    }

    return 0;
}