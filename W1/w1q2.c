#include<stdio.h>
#include<omp.h>

int main(){
    int numthreads;
    printf("Enter number of threads you wish to utilize:\n");
    scanf("%d",&numthreads);
    omp_set_num_threads(numthreads);
    #pragma omp parallel
    {
        printf("Hello from Thread %d\n",omp_get_thread_num());
    }
    return 0;
}