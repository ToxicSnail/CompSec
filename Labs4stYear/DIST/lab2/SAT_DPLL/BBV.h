#ifndef BBV_H
#define BBV_H

#include <iostream>
using namespace std;

typedef unsigned char byte;

class X
{
    byte* ptr;//указатель на ячейку, где хранится нужный бит
    int index;//номер бита в ячейке
public:
    X();//конструктор по умолчаню
    X(byte* vec, int k);//конструктор с параметрами
    X operator=(int k);//перегрузка оператора присвоения
    operator int();//перегрузка типа
    X operator=(X& v);
};


class BBV
{
    friend X;
    byte* vec;//указатель на массив ячеек
    int size;//количество ячеек
    int len;//количество разрядов
public:
    ~BBV();//деструктор
    BBV();//конструктор по умоланию
    BBV(int size);//конструктор с параметром. в качетве параметра нужная длина вектора
    BBV(const char* str);//конструктор с параметром. в качестве параметра строка, которую преобразуем в вектор
    BBV(BBV& V);//конструктор копирования
    void Init(const char* str);//метод инициализации вектора от строки
    void Set0(int k);//метод установки k-ого разряда в 0
    void Set1(int k);//метод установки k-ого разряда в 1
    BBV operator = (BBV& V);//перегрузка оператора присвоения
    BBV operator = (const char* str);//перегрузка оператора присвоения для работы со строками
    bool operator ==(BBV& V);//логическое равенство ввекторов
    BBV operator | (BBV& V);//логическое сложение векторов
    BBV operator & (BBV& V);//логическое умножение векторов
    BBV operator ^ (BBV& V);//операция сумма по модулю два
    BBV operator ~ ();//инверсия вектора
    BBV operator >> (int k);//сдвиг вправо
    BBV operator << (int k);//сдвиг влево
    X operator [] (int k);//перегрузка операции []
    operator char*();//перегрузка операции тип
    int getWeight();//метод для получения веса вектора
    int getSize();//метод для получения размера вектора
    friend ostream & operator << (ostream &r, BBV& V);
    friend istream & operator >> (istream &r, BBV& V);
};



#endif // BBV_H
