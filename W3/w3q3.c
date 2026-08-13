#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

#define N 10

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

void test_schedule(const char* name) {
    int prime_count = 0;
    double start = omp_get_wtime();

    printf("Schedule: %s \n", name);

    if (name[0] == 'S') {
        #pragma omp parallel for schedule(static, 2) reduction(+:prime_count)
        for (int i = 1; i <= N; i++) {
            if (is_prime(i)) prime_count++;
            printf("Thread %d processed number %d\n", omp_get_thread_num(), i);
        }
    } else if (name[0] == 'D') {
        #pragma omp parallel for schedule(dynamic, 2) reduction(+:prime_count)
        for (int i = 1; i <= N; i++) {
            if (is_prime(i)) prime_count++;
            printf("Thread %d processed number %d\n", omp_get_thread_num(), i);
        }
    } else if (name[0] == 'G') {
        #pragma omp parallel for schedule(guided, 2) reduction(+:prime_count)
        for (int i = 1; i <= N; i++) {
            if (is_prime(i)) prime_count++;
            printf("Thread %d processed number %d\n", omp_get_thread_num(), i);
        }
    }

    double end = omp_get_wtime();
    printf("Primes found: %d | Time taken: %f seconds\n\n", prime_count, end - start);
}

int main() {
    omp_set_num_threads(3);
    test_schedule("Static");
    test_schedule("Dynamic");
    test_schedule("Guided");
    return 0;
}