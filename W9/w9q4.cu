#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void rowPowers(long long *a, long long *b, int m, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < m && col < n) {
        int idx = row * n + col;
        long long result = 1;

        for (int k = 0; k <= row; k++)
            result *= a[idx];

        b[idx] = result;
    }
}

int main() {
    int m, n;
    printf("Enter rows and columns: ");
    scanf("%d%d", &m, &n);

    int size = m * n * sizeof(long long);
    long long *a = (long long *)malloc(size);
    long long *b = (long long *)malloc(size);
    long long *da, *db;

    printf("Enter matrix:\n");
    for (int i = 0; i < m * n; i++) scanf("%lld", &a[i]);

    cudaMalloc((void **)&da, size);
    cudaMalloc((void **)&db, size);

    cudaMemcpy(da, a, size, cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid((n + 15) / 16, (m + 15) / 16);

    rowPowers<<<grid, block>>>(da, db, m, n);

    cudaMemcpy(b, db, size, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%lld ", b[i * n + j]);
        printf("\n");
    }

    cudaFree(da);
    cudaFree(db);
    free(a);
    free(b);
    return 0;
}