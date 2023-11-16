#include <stdio.h>
#include <stdbool.h>

int replace(char *text, char *alfa, char *beta);                                            //задаем функцию замены 
int search(char *text, char *alfa);                                                         //задаем функцию поиска

int main()
{
    char text[100+1] = " aaabab";                                                           //задаем начальное слово необходимое для работы, где преобразования
                                                                                            //происходят путем: А1 -> А2

    char alfa[10][10] = {"**a", "**b", "**|", "**", "|a", "|*", "|b", "*b", "*a", " ",};    //задаем подстановку A1
    char beta[10][10] = {"a**", "b**", "|**", " ", "a|", "*|", "b|", "b**", "a|*", "*"};    //задаем подстановку А2

    int ix = 0;                                                                             //задаем итерационное значение
    int tmp = 0;                                                                            //задаем временнное значение для получения результата о перестановке

    while (true)                                                                            //
    {                                                                                       //выполняем пока правда:
        tmp = replace(text, alfa[ix], beta[ix]);                                            //Выполняем замену А1 -> А2 посимвольно в text
        if (!tmp)                                                                           //если не замена, то 
        {                                                                                   //Смотрим на то, выполняется ли сейчас конечная подстановка,
            if (ix == 3)                                                                    //если выполняется(выполнена), то завершаем подстановки.
                break;                                                                      //
            else                                                                            //
                ix = 0;                                                                     //Иначе, обнуляем индекс, который влияет на подставляемые значения
        }                                                                                   //в подстановках
        else                                                                                //
            ix++;                                                                           //Если же мы даже не зашли в цикл, то увеличиваем индекс
    }                                                                                       //
    printf("%s\n", text);                                                                   //Теперь, мы имеем конечное измененное значение, выводим его
}

int search(char *text, char *alfa)                                                          //поиск подстроки А1 в строке text
{                                                                                           //
    if (!(text && alfa))                                                                    //
        return -1;                                                                          //

    char *ax = NULL;                                                                        //
    char *tx = NULL;                                                                        //
    bool isHere = false;                                                                    //
    int i = 0;                                                                              //
    for (; text[i] != '\0' && (!isHere); i++)                                               //сравниваем две строки посимвольно
    {                                                                                       //
        ax = alfa;                                                                          //
        tx = &text[i];                                                                      //

        while (*ax != '\0' && *tx == *ax)                                                   //
        {                                                                                   //
            tx++;                                                                           //
            ax++;                                                                           //
        }                                                                                   //

        if (*ax == '\0')                                                                    //
            isHere = true;                                                                  //
    }                                                                                       //

    if (isHere)                                                                             //
        return i - 1;                                                                       //
    else                                                                                    //
        return -1;                                                                          //на случай, если подстроки в строке нет
}

int replace(char *text, char *alfa, char *beta)                                             //замена подстроки А1 в text на подстроку А2
{                                                                                           //
    if (!(text && alfa && beta))                                                            //если у нас посимвольно равны подстроки, то выходим сразу 
        return -1;                                                                          //
    
    int pos = search(text, alfa);                                                           //находим позицию вхождения, если
    if (pos < 0)                                                                            //она отрицательна, то работа с ней невозможна, 
        return -1;                                                                          //выходим из нее

    char *point = text;                                                                     //создаем указатель на строку text
    int text_len = 0;                                                                       //определяем длину text
    for(text_len; *(point++); text_len++);                                                  //

    point = alfa;                                                                           //указателю присваем значение А1
    int alfa_len = 0;                                                                       //определяем длину А1
    for(alfa_len; *(point++); alfa_len++);                                                  //

    point = beta;                                                                           //указателю присваем значение А2
    int beta_len = 0;                                                                       //определяем длину А2
    for(beta_len; *(point++); beta_len++);                                                  //
    
    int re_len = beta_len - alfa_len;                                                       //определяем разность длин А2 и А1
    for (int i = text_len; i > pos; i--)                                                    //
        text[i + re_len] = text[i];                                                         //
    for (int i = 0; i < beta_len; i++)                                                      //заменяем А2 на А1
        text[pos + i] = beta[i];                                                            //
    return 0;                                                                               //
}                                                                                           //
