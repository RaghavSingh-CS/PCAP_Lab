#include <stdio.h>
#include <cuda_runtime.h>

__global__ void saxpy(float *x, float *y, float a, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
        y[i] = a * x[i] + y[i];
}

int main()
{
    const int N = 5;

    float x[N] = {1, 2, 3, 4, 5};
    float y[N] = {5, 4, 3, 2, 1};
    float a = 2.0f;

    float *d_x, *d_y;

    cudaMalloc(&d_x, N * sizeof(float));
    cudaMalloc(&d_y, N * sizeof(float));

    cudaMemcpy(d_x, x, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, N * sizeof(float), cudaMemcpyHostToDevice);

    saxpy<<<1, N>>>(d_x, d_y, a, N);

    cudaMemcpy(y, d_y, N * sizeof(float), cudaMemcpyDeviceToHost);

    printf("Result: ");

    for (int i = 0; i < N; i++)
        printf("%.1f ", y[i]);

    printf("\n");

    cudaFree(d_x);
    cudaFree(d_y);

    return 0;
}