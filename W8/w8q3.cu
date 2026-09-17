
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void rankSort(int *a, int *result, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        int rank = 0;

        for (int j = 0; j < n; j++) {
            if (a[j] < a[i] || (a[j] == a[i] && j < i))
                rank++;
        }

        result[rank] = a[i];
    }
}

int main() {
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    if (n <= 0)
        return 1;

    int *a = (int *)malloc(n * sizeof(int));
    int *result = (int *)malloc(n * sizeof(int));

    printf("Enter elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    int *d_a, *d_result;

    cudaMalloc((void **)&d_a, n * sizeof(int));
    cudaMalloc((void **)&d_result, n * sizeof(int));

    cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice);

    rankSort<<<(n + 255) / 256, 256>>>(d_a, d_result, n);

    cudaMemcpy(result, d_result, n * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", result[i]);
    printf("\n");

    cudaFree(d_a);
    cudaFree(d_result);

    free(a);
    free(result);

    return 0;
}