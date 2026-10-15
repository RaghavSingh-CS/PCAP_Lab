#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__device__ long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++)
        result *= i;
    return result;
}

__device__ long long digitSum(int n) {
    long long x = n;
    if (x < 0) x = -x;

    long long sum = 0;
    do {
        sum += x % 10;
        x /= 10;
    } while (x > 0);

    return sum;
}

__global__ void transform(int *a, long long *b, int n) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < n && col < n) {
        int idx = row * n + col;

        if (row == col)
            b[idx] = 0;
        else if (row < col)
            b[idx] = factorial(a[idx]);
        else
            b[idx] = digitSum(a[idx]);
    }
}

int main() {
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);

    int sa = n * n * sizeof(int);
    int sb = n * n * sizeof(long long);

    int *a = (int *)malloc(sa);
    long long *b = (long long *)malloc(sb);
    int *da;
    long long *db;

    printf("Enter matrix:\n");
    for (int i = 0; i < n * n; i++) scanf("%d", &a[i]);

    cudaMalloc((void **)&da, sa);
    cudaMalloc((void **)&db, sb);

    cudaMemcpy(da, a, sa, cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid((n + 15) / 16, (n + 15) / 16);

    transform<<<grid, block>>>(da, db, n);

    cudaMemcpy(b, db, sb, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < n; i++) {
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