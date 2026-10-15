#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void csrMultiply(int *values, int *colIndex, int *rowPtr,
                            int *x, int *y, int rows) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows) {
        int sum = 0;
        for (int j = rowPtr[row]; j < rowPtr[row + 1]; j++)
            sum += values[j] * x[colIndex[j]];
        y[row] = sum;
    }
}

int main() {
    int m, n;
    printf("Enter rows and columns: ");
    scanf("%d%d", &m, &n);

    int *a = (int *)malloc(m * n * sizeof(int));
    int *x = (int *)malloc(n * sizeof(int));
    int *y = (int *)malloc(m * sizeof(int));

    printf("Enter sparse matrix:\n");
    for (int i = 0; i < m * n; i++) scanf("%d", &a[i]);

    printf("Enter vector:\n");
    for (int i = 0; i < n; i++) scanf("%d", &x[i]);

    int nnz = 0;
    for (int i = 0; i < m * n; i++)
        if (a[i] != 0) nnz++;

    int *values = (int *)malloc(nnz * sizeof(int));
    int *colIndex = (int *)malloc(nnz * sizeof(int));
    int *rowPtr = (int *)malloc((m + 1) * sizeof(int));

    int count = 0;
    rowPtr[0] = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i * n + j] != 0) {
                values[count] = a[i * n + j];
                colIndex[count] = j;
                count++;
            }
        }
        rowPtr[i + 1] = count;
    }

    printf("CSR Values: ");
    for (int i = 0; i < nnz; i++) printf("%d ", values[i]);

    printf("\nCSR Column Indices: ");
    for (int i = 0; i < nnz; i++) printf("%d ", colIndex[i]);

    printf("\nCSR Row Pointers: ");
    for (int i = 0; i <= m; i++) printf("%d ", rowPtr[i]);
    printf("\n");

    int *dv, *dc, *dr, *dx, *dy;

    cudaMalloc((void **)&dv, (nnz ? nnz : 1) * sizeof(int));
    cudaMalloc((void **)&dc, (nnz ? nnz : 1) * sizeof(int));
    cudaMalloc((void **)&dr, (m + 1) * sizeof(int));
    cudaMalloc((void **)&dx, n * sizeof(int));
    cudaMalloc((void **)&dy, m * sizeof(int));

    if (nnz > 0) {
        cudaMemcpy(dv, values, nnz * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(dc, colIndex, nnz * sizeof(int), cudaMemcpyHostToDevice);
    }

    cudaMemcpy(dr, rowPtr, (m + 1) * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dx, x, n * sizeof(int), cudaMemcpyHostToDevice);

    csrMultiply<<<(m + 255) / 256, 256>>>(dv, dc, dr, dx, dy, m);

    cudaMemcpy(y, dy, m * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < m; i++) printf("%d\n", y[i]);

    cudaFree(dv);
    cudaFree(dc);
    cudaFree(dr);
    cudaFree(dx);
    cudaFree(dy);

    free(a);
    free(x);
    free(y);
    free(values);
    free(colIndex);
    free(rowPtr);
    return 0;
}