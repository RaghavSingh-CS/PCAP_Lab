#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;

    printf("Enter size of arrays: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));
    int *c = malloc(n * sizeof(int));

    printf("Enter elements of array A:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    printf("Enter elements of array B:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &b[i]);
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];

        printf("Thread %d computed c[%d] = %d\n",
               omp_get_thread_num(), i, c[i]);
    }

    printf("\nResultant array:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", c[i]);
    }

    printf("\n");

    free(a);
    free(b);
    free(c);

    return 0;
}