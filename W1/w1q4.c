#include<stdio.h>
#include<omp.h>

int main(){
    int n;
    printf("Enter number of Elements \n");
    scanf("%d",&n);
    int a[n],b[n];
    printf("Enter elements of the 1st array:\n");
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    printf("Enter elements of the second array:\n");
    for(int i=0;i<n;i++){
        scanf("%d",&b[i]);
    }
    int result[n];
    #pragma omp parallel
    {
        #pragma omp for
        for(int i=0;i<n;i++){
            result[i]=a[i]+b[i];
        }
    }
    printf("Resultant Array is:\n");
    for(int i=0;i<n;i++){
        printf("%d\t",result[i]);
    }
    printf("\n");
}