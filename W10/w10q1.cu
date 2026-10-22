
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define MAX_MASK 256

__constant__ float d_mask[MAX_MASK];

__global__ void convolution(const float *input, float *output, int n, int m) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n) return;

    float sum = 0.0f;
    int radius = m / 2;

    for (int j = 0; j < m; j++) {
        int idx = i + j - radius;
        if (idx >= 0 && idx < n)
            sum += input[idx] * d_mask[j];
    }

    output[i] = sum;
}

int main() {
    int n, m;
    printf("Enter input array size: ");
    scanf("%d", &n);
    printf("Enter odd mask size (max 256): ");
    scanf("%d", &m);

    if (n <= 0 || m <= 0 || m > MAX_MASK || m % 2 == 0) {
        printf("Invalid sizes\n");
        return 1;
    }

    float *input = (float *)malloc(n * sizeof(float));
    float *mask = (float *)malloc(m * sizeof(float));
    float *output = (float *)malloc(n * sizeof(float));

    printf("Enter input elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%f", &input[i]);

    printf("Enter mask elements:\n");
    for (int i = 0; i < m; i++)
        scanf("%f", &mask[i]);

    float *d_input, *d_output;
    cudaMalloc((void **)&d_input, n * sizeof(float));
    cudaMalloc((void **)&d_output, n * sizeof(float));

    cudaMemcpy(d_input, input, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(d_mask, mask, m * sizeof(float));

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    convolution<<<blocks, threads>>>(d_input, d_output, n, m);
    cudaDeviceSynchronize();

    cudaMemcpy(output, d_output, n * sizeof(float), cudaMemcpyDeviceToHost);

    printf("Convolution result:\n");
    for (int i = 0; i < n; i++)
        printf("%.2f ", output[i]);
    printf("\n");

    cudaFree(d_input);
    cudaFree(d_output);
    free(input);
    free(mask);
    free(output);

    return 0;
}
