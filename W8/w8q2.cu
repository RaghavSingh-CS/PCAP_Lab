
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void oddEvenSort(int *a, int n, int phase) {
    int i = 2 * (blockIdx.x * blockDim.x + threadIdx.x) + phase;

    if (i + 1 < n && a[i] > a[i + 1]) {
        int temp = a[i];
        a[i] = a[i + 1];
        a[i + 1] = temp;
    }
}

int main() {
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    if (n <= 0)
        return 1;

    int *a = (int *)malloc(n * sizeof(int));

    printf("Enter elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    int *d_a;

    cudaMalloc((void **)&d_a, n * sizeof(int));
    cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice);

    int blocks = ((n + 1) / 2 + 255) / 256;

    for (int phase = 0; phase < n; phase++)
        oddEvenSort<<<blocks, 256>>>(d_a, n, phase % 2);

    cudaMemcpy(a, d_a, n * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    cudaFree(d_a);
    free(a);

    return 0;
}