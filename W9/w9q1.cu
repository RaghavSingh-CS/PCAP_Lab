#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void addRows(int *a, int *b, int *c, int m, int n) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m) {
        for (int j = 0; j < n; j++) {
            int idx = row * n + j;
            c[idx] = a[idx] + b[idx];
        }
    }
}

__global__ void addColumns(int *a, int *b, int *c, int m, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (col < n) {
        for (int i = 0; i < m; i++) {
            int idx = i * n + col;
            c[idx] = a[idx] + b[idx];
        }
    }
}

__global__ void addElements(int *a, int *b, int *c, int m, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < m && col < n) {
        int idx = row * n + col;
        c[idx] = a[idx] + b[idx];
    }
}

int main() {
    int m, n, choice;

    printf("Enter rows and columns: ");
    scanf("%d%d", &m, &n);

    if (m <= 0 || n <= 0) return 1;

    int size = m * n * sizeof(int);

    int *a = (int *)malloc(size);
    int *b = (int *)malloc(size);
    int *c = (int *)malloc(size);

    int *da, *db, *dc;

    printf("Enter matrix A:\n");
    for (int i = 0; i < m * n; i++)
        scanf("%d", &a[i]);

    printf("Enter matrix B:\n");
    for (int i = 0; i < m * n; i++)
        scanf("%d", &b[i]);

    cudaMalloc((void **)&da, size);
    cudaMalloc((void **)&db, size);
    cudaMalloc((void **)&dc, size);

    cudaMemcpy(da, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, size, cudaMemcpyHostToDevice);

    printf("1. Row-wise Addition\n");
    printf("2. Column-wise Addition\n");
    printf("3. Element-wise Addition\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    dim3 block(16, 16);
    dim3 grid((n + 15) / 16, (m + 15) / 16);

    switch (choice) {
        case 1:
            addRows<<<(m + 255) / 256, 256>>>(da, db, dc, m, n);
            break;

        case 2:
            addColumns<<<(n + 255) / 256, 256>>>(da, db, dc, m, n);
            break;

        case 3:
            addElements<<<grid, block>>>(da, db, dc, m, n);
            break;

        default:
            printf("Invalid choice\n");
            cudaFree(da);
            cudaFree(db);
            cudaFree(dc);
            free(a);
            free(b);
            free(c);
            return 1;
    }

    cudaMemcpy(c, dc, size, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", c[i * n + j]);
        printf("\n");
    }

    cudaFree(da);
    cudaFree(db);
    cudaFree(dc);

    free(a);
    free(b);
    free(c);

    return 0;
}