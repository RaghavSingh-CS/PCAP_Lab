#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void reverse(int *input, int *output, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
        output[n - 1 - i] = input[i];
}

int main()
{
    const int N = 10;

    int *input = (int *)malloc(N * sizeof(int));
    int *output = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
        input[i] = rand() % 100;

    int *d_input, *d_output;

    cudaMalloc(&d_input, N * sizeof(int));
    cudaMalloc(&d_output, N * sizeof(int));

    cudaMemcpy(d_input, input, N * sizeof(int), cudaMemcpyHostToDevice);

    int threads = 32;
    int blocks = (N + threads - 1) / threads;

    reverse<<<blocks, threads>>>(d_input, d_output, N);

    cudaMemcpy(output, d_output, N * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Original: ");
    for (int i = 0; i < N; i++)
        printf("%d ", input[i]);

    printf("\nReversed: ");
    for (int i = 0; i < N; i++)
        printf("%d ", output[i]);

    printf("\nThreads per block: %d\n", threads);
    printf("Blocks: %d\n", blocks);

    cudaFree(d_input);
    cudaFree(d_output);

    free(input);
    free(output);

    return 0;
}