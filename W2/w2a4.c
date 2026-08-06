#include <stdio.h>
#include <omp.h>

int is_prime(int number) {
    if (number < 2) {
        return 0;
    }

    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main() {
    int lower, upper;

    printf("Enter lower and upper limits: ");
    scanf("%d %d", &lower, &upper);

    printf("\nPrime numbers between %d and %d:\n", lower, upper);

    #pragma omp parallel for
    for (int number = lower; number <= upper; number++) {
        if (is_prime(number)) {
            #pragma omp critical
            {
                printf("%d found by Thread %d\n",
                       number, omp_get_thread_num());
            }
        }
    }

    return 0;
}