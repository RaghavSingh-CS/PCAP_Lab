#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void repeatCharacters(char *a, int *b, int *offset,
                                 char *output, int total) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < total) {
        for (int j = 0; j < b[idx]; j++)
            output[offset[idx] + j] = a[idx];
    }
}

int main() {
    int m, n;
    printf("Enter rows and columns: ");
    scanf("%d%d", &m, &n);

    int total = m * n;

    char *a = (char *)malloc(total * sizeof(char));
    int *b = (int *)malloc(total * sizeof(int));
    int *offset = (int *)malloc(total * sizeof(int));

    printf("Enter character matrix:\n");
    for (int i = 0; i < total; i++)
        scanf(" %c", &a[i]);

    printf("Enter integer matrix:\n");
    for (int i = 0; i < total; i++)
        scanf("%d", &b[i]);

    int length = 0;

    for (int i = 0; i < total; i++) {
        if (b[i] < 0) {
            printf("Repetition counts must be nonnegative.\n");
            free(a);
            free(b);
            free(offset);
            return 1;
        }

        offset[i] = length;
        length += b[i];
    }

    char *output = (char *)malloc((length + 1) * sizeof(char));

    char *da, *dout;
    int *db, *doffset;

    cudaMalloc((void **)&da, total * sizeof(char));
    cudaMalloc((void **)&db, total * sizeof(int));
    cudaMalloc((void **)&doffset, total * sizeof(int));
    cudaMalloc((void **)&dout, (length + 1) * sizeof(char));

    cudaMemcpy(da, a, total * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, total * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(doffset, offset, total * sizeof(int), cudaMemcpyHostToDevice);

    repeatCharacters<<<(total + 255) / 256, 256>>>(
        da, db, doffset, dout, total
    );

    cudaMemcpy(output, dout, length * sizeof(char),
               cudaMemcpyDeviceToHost);

    output[length] = '\0';

    printf("Output String: %s\n", output);

    cudaFree(da);
    cudaFree(db);
    cudaFree(doffset);
    cudaFree(dout);

    free(a);
    free(b);
    free(offset);
    free(output);
    return 0;
}