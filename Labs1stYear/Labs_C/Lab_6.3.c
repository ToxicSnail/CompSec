#include <stdio.h>
#define count_main_x 3                  //задаем число элементов в массиве count_main_x
#define count_main_y 3                  //задаем число элементов в массиве count_main_y

void main()
{
    int count;                          //внутренний счетчик

    int X[count_main_x];                //начальный массив X размером, введенным с консоли
    int Y[count_main_y];                //начальный массив Y размером, введенным с консоли
    int Z[count_main_y + count_main_x]; //конечный массив Z, где у нас будут храниться значения из 2-х массивов.

    printf("Введите массив X: ");       //задаем массив X с консоли поэлементно до размера заданного раньше
    for (count = 0; count < count_main_x; count++)
    {
        if (scanf("%d", &X[count]) != 1)
        {
            printf("bad input in array X");
            return;
        }
    }

    printf("Введите массив Y: ");       //задаем массив Y с консоли поэлементно до размера заданного раньше
    for (count = 0; count < count_main_y; count++)
    {
        if (scanf("%d", &Y[count]) != 1)
        {
            printf("bad input in array Y");
            return;
        }
    }

    for (count = 0; count < count_main_x; count++)      //Записываем первые символы массива Z всеми X
    {
        Z[count] = X[count];
    }
    for (count = 0; count < count_main_y; count++)      //Записываем последние символы массива Z всеми Y
    {
        Z[count+count_main_x] = Y[count];
    }

    for(int i = 0 ; i < count_main_y+count_main_x - 1; i++) {       //сортируем массив Z с помощью пузырьковой сортировки
       for(int j = 0 ; j < count_main_y+count_main_x - i - 1 ; j++) {  
           if(Z[j] > Z[j+1]) {           
                                                                                        // если они идут в неправильном порядке, то  
                                                                                        //  меняем их местами. 
              int temp = Z[j];
              Z[j] = Z[j+1] ;
              Z[j+1] = temp; 
           }
       }
    }

    printf("Конечный массив выглядит так: ");       //выводим конечный массив Z
    for (count = 0; count < count_main_y+count_main_x; count++)
    {
        printf("%d ", Z[count]);
    }
}
// 3. Даны массивы целых чисел X и Y. Построить упорядоченный массив Z путем слияния массивов X и Y.
//Например,   X = {3, 5, 0, 2, 7}, Y = { 2, 9, 1, 2, 5}. Тогда получаем Z = {0, 2, 2,2,3,5,5,7,9}.
