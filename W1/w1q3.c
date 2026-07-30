#include<stdio.h>
#include<omp.h>

int main(){
    int mat[5][5];
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                mat[i][j] = i * 5 + j + 1;
            }

            printf("Row %d initialized by Thread %d\n",
                   i + 1, omp_get_thread_num());
        }
    }
    printf("\nInitialized Matrix:\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            printf("%3d ", mat[i][j]);
        }
        printf("\n");
    }

    return 0;
}