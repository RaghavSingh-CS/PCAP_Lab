
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define ITEMS 5

__constant__ int d_prices[ITEMS];

__global__ void calculateBills(const int *quantities, int *totals,
                               int *grandTotal, int n) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    if (id < n) {
        int sum = 0;

        for (int j = 0; j < ITEMS; j++)
            sum += quantities[id * ITEMS + j] * d_prices[j];

        totals[id] = sum;
        atomicAdd(grandTotal, sum);
    }
}

int main() {
    int prices[ITEMS] = {50, 100, 200, 80, 150};
    const char *items[ITEMS] = {
        "Notebook", "Pen Set", "T-Shirt", "Chocolate", "Water Bottle"
    };

    int n;

    printf("========== SHOPPING MALL ==========\n");
    printf("Item Menu:\n");

    for (int i = 0; i < ITEMS; i++)
        printf("%d. %-15s Rs.%d\n", i + 1, items[i], prices[i]);

    printf("===================================\n");
    printf("Enter number of friends: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid number of friends\n");
        return 1;
    }

    int *quantities = (int *)malloc((size_t)n * ITEMS * sizeof(int));
    int *totals = (int *)malloc((size_t)n * sizeof(int));

    for (int i = 0; i < n; i++) {
        printf("\nFriend %d:\n", i + 1);

        for (int j = 0; j < ITEMS; j++) {
            do {
                printf("Quantity of %s: ", items[j]);
                scanf("%d", &quantities[i * ITEMS + j]);

                if (quantities[i * ITEMS + j] < 0)
                    printf("Quantity cannot be negative\n");
            } while (quantities[i * ITEMS + j] < 0);
        }
    }

    int *d_quantities, *d_totals, *d_grandTotal;
    int grandTotal = 0;

    cudaMalloc((void **)&d_quantities, (size_t)n * ITEMS * sizeof(int));
    cudaMalloc((void **)&d_totals, (size_t)n * sizeof(int));
    cudaMalloc((void **)&d_grandTotal, sizeof(int));

    cudaMemcpyToSymbol(d_prices, prices, ITEMS * sizeof(int));
    cudaMemcpy(d_quantities, quantities, (size_t)n * ITEMS * sizeof(int),
               cudaMemcpyHostToDevice);
    cudaMemset(d_grandTotal, 0, sizeof(int));

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    calculateBills<<<blocks, threads>>>(d_quantities, d_totals, d_grandTotal, n);
    cudaDeviceSynchronize();

    cudaMemcpy(totals, d_totals, (size_t)n * sizeof(int),
               cudaMemcpyDeviceToHost);
    cudaMemcpy(&grandTotal, d_grandTotal, sizeof(int),
               cudaMemcpyDeviceToHost);

    printf("\n========== PURCHASE SUMMARY ==========\n");

    for (int i = 0; i < n; i++)
        printf("Friend %d Total: Rs.%d\n", i + 1, totals[i]);

    printf("--------------------------------------\n");
    printf("Grand Total: Rs.%d\n", grandTotal);
    printf("======================================\n");

    cudaFree(d_quantities);
    cudaFree(d_totals);
    cudaFree(d_grandTotal);

    free(quantities);
    free(totals);

    return 0;
}
