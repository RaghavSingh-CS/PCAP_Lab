
#include <stdio.h>
#include <string.h>
#include <cuda_runtime.h>


__global__ void replaceVowels(char *s, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        char c = s[i];

        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
            s[i] = '*';
    }
}

int main() {
    char s[1024];

    printf("Enter string: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';

    int n = strlen(s);
    char *d_s;

    cudaMalloc((void **)&d_s, (n + 4) * sizeof(char));
    cudaMemset(d_s, 0, (n + 4) * sizeof(char));
    cudaMemcpy(d_s, s, n + 1, cudaMemcpyHostToDevice);

    if (n > 0)
        replaceVowels<<<(n + 255) / 256, 256>>>(d_s, n);

    cudaMemcpy(s, d_s, n + 1, cudaMemcpyDeviceToHost);

    printf("Output: %s\n", s);

    cudaFree(d_s);

    return 0;
}