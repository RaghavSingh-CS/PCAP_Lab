
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

__global__ void expand(char *s, char *result, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        int start = i * n - i * (i - 1) / 2;

        for (int j = i; j < n; j++)
            result[start + j - i] = s[j];
    }
}

int main() {
    char s[1024];

    printf("Enter string: ");
    scanf("%1023s", s);

    int n = strlen(s);
    int size = n * (n + 1) / 2;

    char *result = (char *)malloc(size + 1);
    char *d_s, *d_result;

    cudaMalloc((void **)&d_s, n + 1);
    cudaMalloc((void **)&d_result, size + 1);

    cudaMemcpy(d_s, s, n + 1, cudaMemcpyHostToDevice);

    expand<<<(n + 255) / 256, 256>>>(d_s, d_result, n);

    cudaMemcpy(result, d_result, size, cudaMemcpyDeviceToHost);
    result[size] = '\0';

    printf("Output: %s\n", result);

    cudaFree(d_s);
    cudaFree(d_result);
    free(result);

    return 0;
}