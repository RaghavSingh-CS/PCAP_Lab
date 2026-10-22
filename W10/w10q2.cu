
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define TILE_SIZE 256
#define MAX_MASK 256

__constant__ float d_mask[MAX_MASK];

__global__ void tiledConvolution(const float *input, float *output, int n, int m) {
    extern __shared__ float tile[];

    int tx = threadIdx.x;
    int i = blockIdx.x * blockDim.x + tx;
    int radius = m / 2;
    int sharedSize = blockDim.x + m - 1;

    for (int j = tx; j < sharedSize; j += blockDim.x) {
        int globalIdx = blockIdx.x * blockDim.x + j - radius;
        tile[j] = (globalIdx >= 0 && globalIdx < n) ? input[globalIdx] : 0.0f;
    }

    __syncthreads();

    if (i < n) {
        float sum = 0.0f;

        for (int j = 0; j < m; j++)
            sum += tile[tx + j] * d_mask[j];

        output[i] = sum;
    }
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

    int blocks = (n + TILE_SIZE - 1) / TILE_SIZE;
    size_t sharedBytes = (TILE_SIZE + m - 1) * sizeof(float);

    tiledConvolution<<<blocks, TILE_SIZE, sharedBytes>>>(d_input, d_output, n, m);
    cudaDeviceSynchronize();

    cudaMemcpy(output, d_output, n * sizeof(float), cudaMemcpyDeviceToHost);

    printf("Tiled convolution result:\n");
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
