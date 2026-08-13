#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 1000000
#define BINS 10

int main() {
    int *data = (int*)malloc(SIZE * sizeof(int));
    int hist_seq[BINS] = {0};
    int hist_par[BINS] = {0};

    for (int i = 0; i < SIZE; i++) data[i] = rand() % BINS;

    // Sequential
    double start = omp_get_wtime();
    for (int i = 0; i < SIZE; i++) {
        hist_seq[data[i]]++;
    }
    double t_seq = omp_get_wtime() - start;

    // Parallel
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        #pragma omp atomic
        hist_par[data[i]]++;
    }
    double t_par = omp_get_wtime() - start;

    printf("Sequential Time: %f s\n", t_seq);
    printf("Parallel Time:   %f s\n", t_par);

    printf("\nBin distribution check (Bin 0): Seq = %d, Par = %d\n", hist_seq[0], hist_par[0]);

    free(data);
    return 0;
}