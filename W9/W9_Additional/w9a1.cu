#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void replaceElements(int *a, int *b, int m, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < m && col < n) {
        int idx = row * n + col;
        int sum = 0;

        if (a[idx] % 2 == 0) {
            for (int j = 0; j < n; j++)
                sum += a[row * n + j];
        } else {
            for (int i = 0; i < m; i++)
                sum += a[i * n + col];
        }

        b[idx] = sum;
    }
}

int main() {
    int m, n;
    printf("Enter rows and columns: ");
    scanf("%d%d", &m, &n);

    int size = m * n * sizeof(int);
    int *a = (int *)malloc(size);
    int *b = (int *)malloc(size);
    int *da, *db;

    printf("Enter matrix:\n");
    for (int i = 0; i < m * n; i++) scanf("%d", &a[i]);

    cudaMalloc((void **)&da, size);
    cudaMalloc((void **)&db, size);

    cudaMemcpy(da, a, size, cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid((n + 15) / 16, (m + 15) / 16);

    replaceElements<<<grid, block>>>(da, db, m, n);

    cudaMemcpy(b, db, size, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", b[i * n + j]);
        printf("\n");
    }

    cudaFree(da);
    cudaFree(db);
    free(a);
    free(b);
    return 0;
}