
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

__global__ void repeatString(char *s, char *result, int len, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < len * n)
        result[i] = s[i % len];
}

int main() {
    char s[1024];
    int n;

    printf("Enter string: ");
    scanf("%1023s", s);

    printf("Enter repetition count: ");
    scanf("%d", &n);

    if (n < 0)
        return 1;

    int len = strlen(s);
    size_t size = (size_t)len * n;

    char *result = (char *)malloc(size + 1);
    char *d_s, *d_result;

    cudaMalloc((void **)&d_s, len + 1);
    cudaMalloc((void **)&d_result, size + 1);

    cudaMemcpy(d_s, s, len + 1, cudaMemcpyHostToDevice);

    if (size > 0)
        repeatString<<<(size + 255) / 256, 256>>>(d_s, d_result, len, n);

    cudaMemcpy(result, d_result, size, cudaMemcpyDeviceToHost);
    result[size] = '\0';

    printf("Output: %s\n", result);

    cudaFree(d_s);
    cudaFree(d_result);
    free(result);

    return 0;
}