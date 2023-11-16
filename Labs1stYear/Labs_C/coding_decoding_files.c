#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define CODING "coding.txt" // Файл "coding.txt" нужен чтобы видеть что хотел закодировать пользователь
#define CODE "code.txt"  // Файл "interval.txt" нужен для того чтобы видеть помежуток(во что была закодирована строка)
#define DECODING "decoding.txt" // Файл "decoding.txt" нужен чтобы проверить как разкодировалась строка пользователя


bool fgets_my(char* str, int size); /* Функция fgets_my нужна чтобы удалять символ переноса строки
                                      когда пользователь вводит строку с консоли */
int main()
{
    FILE *ptrFile = NULL;
    FILE *ptrCopyF = NULL;
    FILE *Code = NULL;

    char element;
    char a;
    char b;
    char c;
    char key;
    key = 'a';                      //устанавливаем key
    char str_to_coding[100];

    printf("Input string: ");
    if(!fgets_my(str_to_coding,100)){
        printf("Failed to remove line break character.");
        return 0;
        exit(0);
    }

    printf("Key - %c", key);
    printf("\n\n");

    // Записываем строку в файл
    ptrFile = fopen(CODING, "w");
    if(ptrFile){
        int fp = fputs(str_to_coding,ptrFile);
        if(fp == -1){
            printf("Failed to write string in file");
            exit(0);
        }
        //printf("String is written in file.\n");
        fclose(ptrFile);
    }else{
        printf("Failed to open on read.");
        exit(0);
    }

    //Кодируем файл
    ptrFile = fopen(CODING, "r");
    ptrCopyF = fopen(CODE,"w");
    if(ptrFile && ptrCopyF){ //Если указатели не пусты, значит файлы открыты на чтение

        while(fscanf(ptrFile,"%c",&element)!= -1){ //Пока не конец файла
            element = element ^ key;  //Используем сумму по модулю 2 для кодирования строки
            fprintf(ptrCopyF, "%c", element);
        }

        fclose(ptrFile);  //Обязательно закрываем файл
        fclose(ptrCopyF); //Обязательно закрываем файл
        //printf("Coding Succesful.\n");
    }
    else {
        printf("Failed to open files for coding.\n");
        exit(0);
    }

    //Раскодируем файл
    ptrFile = fopen(CODE, "r");
    ptrCopyF = fopen(DECODING,"w");

    if(ptrFile && ptrCopyF){   //Если указатели не пусты, значит файлы открыты на чтение

        while(fscanf(ptrFile,"%c",&element)!= -1){ //Пока не конец файла
            element = element ^ key;  //Используем сумму по модулю 2 для раскодирования строки
            fprintf(ptrCopyF, "%c", element);
        }

        fclose(ptrFile);  //Обязательно закрываем файл
        fclose(ptrCopyF); //Обязательно закрываем файл
    }
    else {
        printf("Failed to open files for decoding.\n");
        exit(0);
    }

    //Сравниваем файлы

    ptrFile = fopen(CODING,"r");
    ptrCopyF = fopen(DECODING,"r");
    Code = fopen(CODE,"r");

    if(ptrFile && ptrCopyF && Code){

        printf(" What coding: ");
        while(fscanf(ptrFile, "%c",&a)!= -1)
            printf("%c", a);
        printf("\n");

        printf(" Coding: ");
        while(fscanf(Code, "%c",&b)!= -1)
            printf("%c", b);
        printf("\n");

        printf(" Decoding: ");
        while(fscanf(ptrCopyF, "%c",&c)!= -1)
            printf("%c", c);
        printf("\n");

        fclose(ptrFile);     //Обязательно закрываем файл
        fclose(ptrCopyF);    //Обязательно закрываем файл
        fclose(Code); //Обязательно закрываем файл
    }
    else{
        printf("We can't open files to compare.\n");
        exit(0);
    }
    printf("\n");
    if(a == c)
        printf("Files have the same content.\n");
    else
        printf("Files have different content.\n");

    return 0;
}

bool fgets_my(char* str, int size) /* Функция fgets_my нужна чтобы удалять символ переноса строки
                                      когда пользователь вводит строку с консоли */
{
    char* fgets_result = fgets(str, size, stdin);
    if (fgets_result != str)
        return false;

    int len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
    return true;
}
