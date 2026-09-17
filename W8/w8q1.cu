
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void convolution(float *N, float *M, float *P, int width, int mask_width) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < width) {
        float sum = 0;
        int start = i - mask_width / 2;

        for (int j = 0; j < mask_width; j++) {
            int index = start + j;
            if (index >= 0 && index < width)
                sum += N[index] * M[j];
        }

        P[i] = sum;
    }
}

int main() {
    int n, m;

    printf("Enter array size: ");
    scanf("%d", &n);

    printf("Enter odd mask size: ");
    scanf("%d", &m);

    if (n <= 0 || m <= 0 || m % 2 == 0)
        return 1;

    float *N = (float *)malloc(n * sizeof(float));
    float *M = (float *)malloc(m * sizeof(float));
    float *P = (float *)malloc(n * sizeof(float));

    printf("Enter array elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%f", &N[i]);

    printf("Enter mask elements:\n");
    for (int i = 0; i < m; i++)
        scanf("%f", &M[i]);

    float *d_N, *d_M, *d_P;

    cudaMalloc((void **)&d_N, n * sizeof(float));
    cudaMalloc((void **)&d_M, m * sizeof(float));
    cudaMalloc((void **)&d_P, n * sizeof(float));

    cudaMemcpy(d_N, N, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M, M, m * sizeof(float), cudaMemcpyHostToDevice);

    convolution<<<(n + 255) / 256, 256>>>(d_N, d_M, d_P, n, m);

    cudaMemcpy(P, d_P, n * sizeof(float), cudaMemcpyDeviceToHost);

    printf("Result: ");
    for (int i = 0; i < n; i++)
        printf("%.2f ", P[i]);
    printf("\n");

    cudaFree(d_N);
    cudaFree(d_M);
    cudaFree(d_P);

    free(N);
    free(M);
    free(P);

    return 0;
}