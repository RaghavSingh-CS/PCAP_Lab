#include <stdio.h>
#include <cuda_runtime.h>

__global__ void vectorAdd(int *a, int *b, int *c, int n)
{
    int i = threadIdx.x;

    if (i < n)
        c[i] = a[i] + b[i];
}

__global__ void dotProduct(int *a, int *b, int *result, int n)
{
    int i = threadIdx.x;

    if (i < n)
        atomicAdd(result, a[i] * b[i]);
}

int main()
{
    const int N = 5;

    int a[N] = {1, 2, 3, 4, 5};
    int b[N] = {5, 4, 3, 2, 1};
    int c[N];
    int result = 0;

    int *d_a, *d_b, *d_c, *d_result;

    cudaMalloc(&d_a, N * sizeof(int));
    cudaMalloc(&d_b, N * sizeof(int));
    cudaMalloc(&d_c, N * sizeof(int));
    cudaMalloc(&d_result, sizeof(int));

    cudaMemcpy(d_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, &result, sizeof(int), cudaMemcpyHostToDevice);

    vectorAdd<<<1, N>>>(d_a, d_b, d_c, N);
    dotProduct<<<1, N>>>(d_a, d_b, d_result, N);

    cudaMemcpy(c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(&result, d_result, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Vector Addition: ");
    for (int i = 0; i < N; i++)
        printf("%d ", c[i]);

    printf("\nDot Product: %d\n", result);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFree(d_result);

    return 0;
}