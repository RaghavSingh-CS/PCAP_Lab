
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define TILE_SIZE 16

__global__ void matrixMultiply(const float *A, const float *B, float *C,
                               int rowsA, int colsA, int colsB) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    __shared__ float tileA[TILE_SIZE][TILE_SIZE];
    __shared__ float tileB[TILE_SIZE][TILE_SIZE];

    float sum = 0.0f;

    for (int t = 0; t < (colsA + TILE_SIZE - 1) / TILE_SIZE; t++) {
        int aCol = t * TILE_SIZE + threadIdx.x;
        int bRow = t * TILE_SIZE + threadIdx.y;

        tileA[threadIdx.y][threadIdx.x] =
            (row < rowsA && aCol < colsA) ? A[row * colsA + aCol] : 0.0f;

        tileB[threadIdx.y][threadIdx.x] =
            (bRow < colsA && col < colsB) ? B[bRow * colsB + col] : 0.0f;

        __syncthreads();

        for (int k = 0; k < TILE_SIZE; k++)
            sum += tileA[threadIdx.y][k] * tileB[k][threadIdx.x];

        __syncthreads();
    }

    if (row < rowsA && col < colsB)
        C[row * colsB + col] = sum;
}

int main() {
    int rowsA, colsA, rowsB, colsB;

    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &rowsA, &colsA);

    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &rowsB, &colsB);

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0 || colsA != rowsB) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    size_t sizeA = (size_t)rowsA * colsA * sizeof(float);
    size_t sizeB = (size_t)rowsB * colsB * sizeof(float);
    size_t sizeC = (size_t)rowsA * colsB * sizeof(float);

    float *A = (float *)malloc(sizeA);
    float *B = (float *)malloc(sizeB);
    float *C = (float *)malloc(sizeC);

    printf("Enter Matrix A elements:\n");
    for (int i = 0; i < rowsA * colsA; i++)
        scanf("%f", &A[i]);

    printf("Enter Matrix B elements:\n");
    for (int i = 0; i < rowsB * colsB; i++)
        scanf("%f", &B[i]);

    float *d_A, *d_B, *d_C;

    cudaMalloc((void **)&d_A, sizeA);
    cudaMalloc((void **)&d_B, sizeB);
    cudaMalloc((void **)&d_C, sizeC);

    cudaMemcpy(d_A, A, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeB, cudaMemcpyHostToDevice);

    dim3 block(TILE_SIZE, TILE_SIZE);
    dim3 grid((colsB + TILE_SIZE - 1) / TILE_SIZE,
              (rowsA + TILE_SIZE - 1) / TILE_SIZE);

    matrixMultiply<<<grid, block>>>(d_A, d_B, d_C, rowsA, colsA, colsB);
    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, sizeC, cudaMemcpyDeviceToHost);

    printf("Resultant Matrix:\n");
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++)
            printf("%.2f ", C[i * colsB + j]);
        printf("\n");
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(A);
    free(B);
    free(C);

    return 0;
}
