#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

void merge_sort_seq(int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge_sort_par(int *arr, int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;
        if (depth > 0) {
            #pragma omp task shared(arr)
            merge_sort_par(arr, l, m, depth - 1);
            #pragma omp task shared(arr)
            merge_sort_par(arr, m + 1, r, depth - 1);
            #pragma omp taskwait
        } else {
            merge_sort_seq(arr, l, m);
            merge_sort_seq(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int n = 100000;
    int *arr1 = (int*)malloc(n * sizeof(int));
    int *arr2 = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr1[i] = rand() % 100000;
        arr2[i] = arr1[i];
    }

    // Sequential
    double start = omp_get_wtime();
    merge_sort_seq(arr1, 0, n - 1);
    double t_seq = omp_get_wtime() - start;

    // Parallel
    int num_threads = 4;
    omp_set_num_threads(num_threads);
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort_par(arr2, 0, n - 1, 4);
    }
    double t_par = omp_get_wtime() - start;

    double speedup = t_seq / t_par;
    double efficiency = speedup / num_threads;

    printf("Input Size: %d\n", n);
    printf("Threads: %d\n", num_threads);
    printf("Sequential Time: %f s\n", t_seq);
    printf("Parallel Time:   %f s\n", t_par);
    printf("Speedup:         %.2fx\n", speedup);
    printf("Efficiency:      %.2f%%\n", efficiency * 100);

    free(arr1); free(arr2);
    return 0;
}