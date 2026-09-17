#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

__global__ void euclidean(float *a, float *b, float *sum, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        float d = a[i] - b[i];
        atomicAdd(sum, d * d);
    }
}

int main()
{
    const int N = 1000;

    float *a = (float *)malloc(N * sizeof(float));
    float *b = (float *)malloc(N * sizeof(float));

    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 10;
        b[i] = rand() % 10;
    }

    float *d_a, *d_b, *d_sum;
    float sum = 0.0f;

    cudaMalloc(&d_a, N * sizeof(float));
    cudaMalloc(&d_b, N * sizeof(float));
    cudaMalloc(&d_sum, sizeof(float));

    cudaMemcpy(d_a, a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sum, &sum, sizeof(float), cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    euclidean<<<blocks, threads>>>(d_a, d_b, d_sum, N);

    cudaMemcpy(&sum, d_sum, sizeof(float), cudaMemcpyDeviceToHost);

    printf("N: %d\n", N);
    printf("Threads per block: %d\n", threads);
    printf("Blocks: %d\n", blocks);
    printf("Euclidean Distance: %f\n", sqrtf(sum));

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_sum);

    free(a);
    free(b);

    return 0;
}