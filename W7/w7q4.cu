#include <stdio.h>
#include <math.h>
#include <cuda_runtime.h>

__global__ void sine(float *a, float *result, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
        result[i] = sinf(a[i]);
}

int main()
{
    const int N = 5;

    float a[N] = {
        0.0f,
        0.523599f,
        0.785398f,
        1.570796f,
        3.141593f
    };

    float result[N];

    float *d_a, *d_result;

    cudaMalloc(&d_a, N * sizeof(float));
    cudaMalloc(&d_result, N * sizeof(float));

    cudaMemcpy(d_a, a, N * sizeof(float), cudaMemcpyHostToDevice);

    sine<<<1, N>>>(d_a, d_result, N);

    cudaMemcpy(result, d_result, N * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++)
        printf("sin(%f) = %f\n", a[i], result[i]);

    cudaFree(d_a);
    cudaFree(d_result);

    return 0;
}