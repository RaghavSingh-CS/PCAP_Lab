#include <stdio.h>
#include <omp.h>

#define size 12

int main() {
    int arr[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int total_sum = 0;
    int even_count = 0;

    #pragma omp parallel
    {
        #pragma omp master
        {
            printf("Master Thread: Total threads = %d\n", omp_get_num_threads());
            printf("Array initialized successfully.\n\n");
        }
        #pragma omp barrier
        int local_sum = 0;

        #pragma omp for reduction(+:total_sum)
        for (int i = 0; i < size; i++) {
            total_sum += arr[i];
            local_sum += arr[i];

            if (arr[i] % 2 == 0) {
                #pragma omp atomic
                even_count++;
            }
        }

        #pragma omp critical
        {
            printf("Thread %d: Partial Sum = %d\n", omp_get_thread_num(), local_sum);
        }
    }

    printf("\nFINAL RESULTS\n");
    printf("Total Sum of Elements: %d\n", total_sum);
    printf("Total Count of Even Numbers: %d\n", even_count);

    return 0;
}