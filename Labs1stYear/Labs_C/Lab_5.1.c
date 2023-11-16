#include <math.h>
#include <stdio.h>

void main()
{
    int V, V1;
    int cifra = 1;
    int reversV = 0, count = 0, temp = 0;

    if (scanf("%d", &V) != 1)
    {
        printf("bad input");
        return;
    }                   //считываем начальное число V, проверяя его
    
    V1 = V;
    while (V > 0)   //
    {               //
        count++;    //подсчитываем кол-во символов в числе V
        V = V / 10; //
    }               //

    for (int i = 0; i < count - 1; i++) //
    {                                   //подсчет макимальной "десятки"(домножаемая степеь десятки))
        cifra *= 10;                    //
    }                                   //

    while (V1 > 0)                        //
    {                                     //
        temp = V1 % 10;                   //
        reversV = temp * cifra + reversV; //производится подсчет числа
        V1 = V1 / 10;                     //
        cifra = cifra / 10;               //
    }
    printf("%d", reversV); //вывод перевернутого V
}

// 1. Сформировать число V, в котором, цифры, заданного натурального числа N расположены в обратном порядке. Например, если N = 123, то число V = 321.
