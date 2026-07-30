#include<stdio.h>
#include<omp.h>

int main(){
    int m,n;
    printf("Enter number of rows of the 2D Matrix: ");
    scanf("%d",&m);
    printf("Enter number of columns of the 2D Matrix: ");
    scanf("%d",&n);
    int a[m][n],b[m][n];
    printf("Enter elements of the first matrix:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&a[i][j]);
        }
    }
    printf("Enter elements of the second matrix:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&b[i][j]);
        }
    }
    int result[m][n];
    #pragma omp parallel
    {
        #pragma omp for
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    result[i][j]=a[i][j]+b[i][j];
                }
                printf("Row %d intialized by Thread %d\n",i+1,omp_get_thread_num());
            }
    }
    printf("Final Matrix Resultant:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%d\t",result[i][j]);
        }
        printf("\n");
    }
    return 0;
}