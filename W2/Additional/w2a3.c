#include <stdio.h>
#include <omp.h>

int reverse_number(int number) {
    int reversed = 0;

    while (number > 0) {
        reversed = reversed * 10 + number % 10;
        number = number / 10;
    }

    return reversed;
}

int main() {
    int array[] = {18, 523, 301, 1234, 2, 14, 108, 150, 1928};
    int n = sizeof(array) / sizeof(array[0]);
    int reversed[n];

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        reversed[i] = reverse_number(array[i]);

        printf("Thread %d reversed %d to %d\n",
               omp_get_thread_num(),
               array[i],
               reversed[i]);
    }

    printf("\nOriginal Array:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }

    printf("\n\nReversed Array:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", reversed[i]);
    }

    printf("\n");

    return 0;
}