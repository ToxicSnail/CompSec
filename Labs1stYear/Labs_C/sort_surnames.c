#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void sort_strings(char** surnames, int surnames_count);
void delete_strings(char** surnames, int surnames_count);
void print_strings(char** surnames, int surnames_count);

int main()
{
    int surnames_count = 0; //переменная, в которой содержится количество фамилий
    if (scanf("%d\n", &surnames_count) != 1)    //считывание количества фамилий
    {
        printf("Error with input!");
        return 0;
    }

    char** surnames = (char**)malloc(sizeof(char*) * surnames_count);   //выделяем память под введённое количество фамилий
    if (!surnames)                                                      //если адрес не будет получен, значит, память выделить не получилось
    {
        printf("Failed to allocate memory\n");
        return 0;
    }

    char surname_buffer[64];    //oбъявляет массив surname_buffer из 64 символов и целое число surname_len.
    int surname_len;            //
    for (int i = 0; i < surnames_count; ++i)
    {
        surname_len = 0;
        scanf("%s", surname_buffer);    //считываем фамилии
        for (int j = 0; surname_buffer[j]; ++j)     //длина фамилии
            surname_len++;
        surnames[i] = (char*)malloc(surname_len);   //создаём массив соответсвующей длине фамилии
        if (!surnames[i])   //если адрес не будет получен, значит, память выделить не получилось
            return 0;
        for (int j = 0; surname_buffer[j]; ++j) //этот цикл копирует символы SurnamesBuffer в Surnames[i] посимвольно.
            surnames[i][j] = surname_buffer[j];
    }

    sort_strings(surnames, surnames_count); //сортируем массив фамилий

    delete_strings(surnames, surnames_count);   //удаляем повторяющиеся строки

    for (int i = 0; i < surnames_count; ++i)
    {
        free(surnames[i]);
    }
    free(surnames);
    return 0;
}

void sort_strings(char** surnames, int surnames_count)
{
    if (!surnames)
        return 0;

    char* temp = NULL;  //временный указатель для хранения строки при сортировке
    int res = 0;    //переменная для хранения результата сравнения строк при сортировке
    for (int i = 0; i < surnames_count - 1; ++i)    //внешний цикл перебирает элементы массива
    {
        for (int j = 0; j < surnames_count - i - 1; ++j)    //внутренний цикл сравнивает текущий элемент с остальными
        {
            res = strcmp(surnames[j], surnames[j + 1]);     //Сравниваем j-ю и (j+1)-ю фамилии
            if (res > 0)    //Если j-я фамилия больше, чем (j+1)-я фамилия, меняем местами указатели на строки
            {
                temp = surnames[j];
                surnames[j] = surnames[j + 1];
                surnames[j + 1] = temp;
            }

        }
    }
}

void delete_strings(char** surnames, int surnames_count)
{
    int i, j;
    if (surnames) 
    {
        for (i = 0; i < surnames_count; i++) // цикл для проверки каждого элемента
        { 
            if (surnames[i] != NULL)
            {
                for (j = i + 1; j < surnames_count;)    // цикл для поиска дубликатов элемента arr[i]
                { 
                    if (surnames[j] != NULL)
                    {
                        if (strcmp(surnames[j], surnames[i]) == 0) // если нашелся дубликат
                        {
                            free(surnames[j]);
                            surnames[j] = NULL;
                        }
                        else j++; // переход к следующему элементу
                    }
                    else j++;
                }
            }
        }
    }
    print_strings(surnames, surnames_count);    //выводим итоговый массив фамилий на экран
}

void print_strings(char** surnames, int surnames_count)
{
    if (!surnames)
        return 0;
    for (int i = 0; i < surnames_count; ++i)
    {
        if (surnames[i] != NULL) printf("\n%s", surnames[i]);
    }
}
