#include <stdio.h>
#include <omp.h>

#define N 12

long long fact(int n) {
    long long res = 1;
    for (int i = 1; i <= n; i++) res *= i;
    return res;
}

int main() {
    double start, end;

    // 1. Critical
    start = omp_get_wtime();
    long long sum_crit = 0;
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        long long f = fact(i);
        #pragma omp critical
        sum_crit += f;
    }
    end = omp_get_wtime();
    printf("Critical Section Time: %f s | Sum: %lld\n", end - start, sum_crit);

    // 2. Atomic
    start = omp_get_wtime();
    long long sum_atomic = 0;
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        long long f = fact(i);
        #pragma omp atomic
        sum_atomic += f;
    }
    end = omp_get_wtime();
    printf("Atomic Construct Time: %f s | Sum: %lld\n", end - start, sum_atomic);

    // 3. Reduction
    start = omp_get_wtime();
    long long sum_red = 0;
    #pragma omp parallel for reduction(+:sum_red)
    for (int i = 1; i <= N; i++) {
        sum_red += fact(i);
    }
    end = omp_get_wtime();
    printf("Reduction Time:        %f s | Sum: %lld\n", end - start, sum_red);

    // 4. Locks
    start = omp_get_wtime();
    long long sum_lock = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        long long f = fact(i);
        omp_set_lock(&lock);
        sum_lock += f;
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    end = omp_get_wtime();
    printf("Locks Time:            %f s | Sum: %lld\n", end - start, sum_lock);

    return 0;
}