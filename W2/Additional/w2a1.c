#include <stdio.h>
#include <omp.h>

long long power(int base, int exponent) {
    long long result = 1;

    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

int main() {
    int number;

    printf("Enter an integer: ");
    scanf("%d", &number);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        long long result = power(number, thread_id);

        printf("Thread %d computed %d^%d = %lld\n",
               thread_id, number, thread_id, result);
    }

    return 0;
}