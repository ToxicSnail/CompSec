#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort_c(int *arr, int n) 
{
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (arr[j] > arr[j + 1]) 
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubble_sort_asm(int *arr, int n) 
{
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            __asm__ volatile (
                "movl (%1, %2, 4), %%eax\n\t"      // arr[j]
                "movl 4(%1, %2, 4), %%ebx\n\t"    // arr[j+1]
                "cmpl %%ebx, %%eax\n\t"           
                "jle no_swap\n\t"                 // Если arr[j] <= arr[j+1], перейти
                "movl %%ebx, (%1, %2, 4)\n\t"     // arr[j+1] -> arr[j]
                "movl %%eax, 4(%1, %2, 4)\n\t"    // arr[j] -> arr[j+1]
                "no_swap:\n\t"
                :                                
                : "r" (arr), "r" (arr), "r" (j)  
                : "%eax", "%ebx"                 
            );
        }
    }
}

int is_sorted(int *arr, int n) 
{
    for (int i = 1; i < n; i++) 
    {
        if (arr[i - 1] > arr[i]) 
        {
            return 0;
        }
    }
    return 1;
}

int main() 
{
    int n;

    printf("Введите количество элементов массива: ");
    scanf("%d", &n);

    int *arr_c = (int *)malloc(n * sizeof(int));
    int *arr_asm = (int *)malloc(n * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < n; i++) 
    {
        int num = rand() % 1000;
        arr_c[i] = num;
        arr_asm[i] = num;
    }

    clock_t start = clock();
    bubble_sort_c(arr_c, n);
    clock_t end = clock();
    double time_c = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    bubble_sort_asm(arr_asm, n);
    end = clock();
    double time_asm = (double)(end - start) / CLOCKS_PER_SEC;

    printf("C sort: %f sec\n", time_c);
    printf("asm sort: %f sec\n", time_asm);

    double time_difference = time_c - time_asm;
    printf("diff: %f sec\n", time_difference);

    free(arr_c);
    free(arr_asm);

    return 0;
}
