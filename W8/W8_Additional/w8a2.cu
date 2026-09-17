
#include <stdio.h>
#include <string.h>
#include <cuda_runtime.h>

__global__ void reverseWords(char *s, char *result, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i >= n)
        return;

    int start = i;
    int end = i;

    while (start > 0 && s[start - 1] != ' ')
        start--;

    while (end < n && s[end] != ' ')
        end++;

    if (s[i] == ' ')
        result[i] = ' ';
    else
        result[i] = s[start + end - 1 - i];
}

int main() {
    char s[1024], result[1024];

    printf("Enter sentence: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';

    int n = strlen(s);
    char *d_s, *d_result;

    cudaMalloc((void **)&d_s, n + 1);
    cudaMalloc((void **)&d_result, n + 1);

    cudaMemcpy(d_s, s, n + 1, cudaMemcpyHostToDevice);

    if (n > 0)
        reverseWords<<<(n + 255) / 256, 256>>>(d_s, d_result, n);

    cudaMemcpy(result, d_result, n, cudaMemcpyDeviceToHost);
    result[n] = '\0';

    printf("Output: %s\n", result);

    cudaFree(d_s);
    cudaFree(d_result);

    return 0;
}
