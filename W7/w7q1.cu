#include <stdio.h>
#include <cuda_runtime.h>

int main()
{
    int count;
    cudaGetDeviceCount(&count);

    printf("Number of CUDA devices: %d\n", count);

    for (int i = 0; i < count; i++)
    {
        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);

        printf("\nDevice %d\n", i);
        printf("Name: %s\n", prop.name);
        printf("Streaming Multiprocessors: %d\n", prop.multiProcessorCount);
        printf("Maximum Threads per Block: %d\n", prop.maxThreadsPerBlock);
        printf("Maximum Thread Dimensions: %d x %d x %d\n",
               prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("Maximum Grid Dimensions: %d x %d x %d\n",
               prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
        printf("Clock Frequency: %d kHz\n", prop.clockRate);
    }

    return 0;
}