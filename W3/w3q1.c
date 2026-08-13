#include <stdio.h>
#include <omp.h>

#define N 10

int main() {
    int sum = 0;
    int i;

    printf("1. SHARED \n");
    sum = 0;
    #pragma omp parallel for shared(sum)
    for (i = 1; i <= N; i++) {
        sum += i; // Race condition will occur here without synchronization
    }
    printf("Shared Sum (Race condition probable): %d\n\n", sum);

    printf("2. PRIVATE \n");
    sum = 100; // Original value
    #pragma omp parallel for private(sum)
    for (i = 1; i <= N; i++) {
        // 'sum' is uninitialized inside each thread
        sum = i;
    }
    printf("Sum after Private clause (outside loop): %d (Unchanged)\n\n", sum);

    printf("3. FIRSTPRIVATE \n");
    sum = 50; // Initial value
    #pragma omp parallel private(i) firstprivate(sum)
    {
        // Each thread gets sum = 50
        sum += omp_get_thread_num();
        printf("Thread %d local sum: %d\n", omp_get_thread_num(), sum);
    }
    printf("Sum after Firstprivate (outside loop): %d\n\n", sum);

    printf(" 4. LASTPRIVATE \n");
    sum = 0;
    int last_val = 0;
    #pragma omp parallel for lastprivate(last_val)
    for (i = 1; i <= N; i++) {
        last_val = i * 10;
    }
    printf("Lastprivate value (from iteration i=%d): %d\n", N, last_val);

    return 0;
}