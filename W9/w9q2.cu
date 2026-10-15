#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void multiplyRows(int *a, int *b, int *c, int m, int k, int n) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m) {
        for (int col = 0; col < n; col++) {
            int sum = 0;

            for (int x = 0; x < k; x++)
                sum += a[row * k + x] * b[x * n + col];

            c[row * n + col] = sum;
        }
    }
}

__global__ void multiplyColumns(int *a, int *b, int *c, int m, int k, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (col < n) {
        for (int row = 0; row < m; row++) {
            int sum = 0;

            for (int x = 0; x < k; x++)
                sum += a[row * k + x] * b[x * n + col];

            c[row * n + col] = sum;
        }
    }
}

__global__ void multiplyElements(int *a, int *b, int *c, int m, int k, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < m && col < n) {
        int sum = 0;

        for (int x = 0; x < k; x++)
            sum += a[row * k + x] * b[x * n + col];

        c[row * n + col] = sum;
    }
}

int main() {
    int m, k, n, choice;

    printf("Enter M K N: ");
    scanf("%d%d%d", &m, &k, &n);

    if (m <= 0 || k <= 0 || n <= 0) return 1;

    int sa = m * k * sizeof(int);
    int sb = k * n * sizeof(int);
    int sc = m * n * sizeof(int);

    int *a = (int *)malloc(sa);
    int *b = (int *)malloc(sb);
    int *c = (int *)malloc(sc);

    int *da, *db, *dc;

    printf("Enter matrix A:\n");
    for (int i = 0; i < m * k; i++)
        scanf("%d", &a[i]);

    printf("Enter matrix B:\n");
    for (int i = 0; i < k * n; i++)
        scanf("%d", &b[i]);

    cudaMalloc((void **)&da, sa);
    cudaMalloc((void **)&db, sb);
    cudaMalloc((void **)&dc, sc);

    cudaMemcpy(da, a, sa, cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, sb, cudaMemcpyHostToDevice);

    printf("1. Row-wise Multiplication\n");
    printf("2. Column-wise Multiplication\n");
    printf("3. Element-wise Multiplication\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    dim3 block(16, 16);
    dim3 grid((n + 15) / 16, (m + 15) / 16);

    switch (choice) {
        case 1:
            multiplyRows<<<(m + 255) / 256, 256>>>(da, db, dc, m, k, n);
            break;

        case 2:
            multiplyColumns<<<(n + 255) / 256, 256>>>(da, db, dc, m, k, n);
            break;

        case 3:
            multiplyElements<<<grid, block>>>(da, db, dc, m, k, n);
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

    cudaMemcpy(c, dc, sc, cudaMemcpyDeviceToHost);

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