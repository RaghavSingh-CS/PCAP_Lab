
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

__global__ void countWord(char *s, char *word, int n, int m, int *count) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i + m > n)
        return;

    if (i > 0 && s[i - 1] != ' ')
        return;

    if (i + m < n && s[i + m] != ' ')
        return;

    for (int j = 0; j < m; j++) {
        if (s[i + j] != word[j])
            return;
    }

    atomicAdd(count, 1);
}

int main() {
    char s[1024], word[100];

    printf("Enter sentence: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';

    printf("Enter word: ");
    scanf("%99s", word);

    int n = strlen(s);
    int m = strlen(word);
    int count = 0;

    char *d_s, *d_word;
    int *d_count;

    cudaMalloc((void **)&d_s, (n + 1) * sizeof(char));
    cudaMalloc((void **)&d_word, (m + 1) * sizeof(char));
    cudaMalloc((void **)&d_count, sizeof(int));

    cudaMemcpy(d_s, s, (n + 1) * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_word, word, (m + 1) * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_count, &count, sizeof(int), cudaMemcpyHostToDevice);

    if (n > 0)
        countWord<<<(n + 255) / 256, 256>>>(d_s, d_word, n, m, d_count);

    cudaMemcpy(&count, d_count, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Occurrences of '%s': %d\n", word, count);

    cudaFree(d_s);
    cudaFree(d_word);
    cudaFree(d_count);

    return 0;
}