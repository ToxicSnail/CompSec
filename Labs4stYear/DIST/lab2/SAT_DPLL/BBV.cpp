// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//#include "pch.h"
#include "BBV.h"
#include <iostream>
#include <cstring>

BBV::~BBV()
{
    if(vec != NULL)
    delete[]vec;
}

BBV::BBV()
{
    vec = NULL;
    size = 0;
    len = 0;
}

BBV::BBV(int size_v)
{
    if (size_v > 0)
    {
        vec = new byte[(size_v - 1) / 8 + 1];//избегаем случай размера кратного восьми
        if (vec != NULL)//если выделилась память, то заполняем поля размера массива, кол-ва ячеек
        {
            len = size_v;
            size = (size_v - 1) / 8 + 1;
            for (int i = 0; i < size; i++)//обнуляем все ячеки полученного ветора
                vec[i] = 0;
        }
        else
            throw 0;
    }
    else
        throw 0;
}

BBV::BBV(const char * str)
{
    if (str != NULL)//если строка существует
    {
        len = strlen(str);//заполняем поля размера вектора и вычисляем кол-во ячеек
        size = (len - 1) / 8 + 1;
        vec = new byte[size];
        if (vec != NULL)
        {
            int i = 0, j = 8, k = 0;
            byte mask = 1;
            vec[0] ^= vec[0];//чистим содержимое первой ячейки
            while (i < len)//проходим по всем битам и записываем в них соответствующее значение из строки
            {
                if (j > 0)
                {
                    if (str[i] != '0')
                        vec[k] |= mask;
                    mask <<= 1;
                    j--;
                    i++;
                }
                else
                {
                    j = 8;
                    k++;
                    vec[k] ^= vec[k];//чистим содержимое ячейки
                    mask = 1;
                }
            }
        }
        else
            throw 0;
    }
    else
        throw 0;
}

BBV::BBV(BBV & V)
{
    if(V.vec != NULL)//проверяем существование вестора V.vec
    {
        size = V.size;//переписываем значение размеров
        len = V.len;
        vec = new byte[size];//захватываем память под новый веторк и переписываем в него значение из переданного вектора
        if (vec != NULL)
        {
            for (int i = 0; i < size; i++)
                vec[i] = V.vec[i];
        }
        else
            throw 0;
    }
}

void BBV::Init(const char * str)
{
    if (str != NULL)//првоериям существует ли строка
    {
        len = strlen(str);//вычисляем размеры для вектора
        size = (len - 1) / 8 + 1;
        if (vec != NULL)//если вектор сущестовал, то мы его удаляем
            delete vec;
        vec = new byte[size];//захватываем память под новый вектор и переписываем в него значения из строки
        if (vec != NULL)
        {
            int i = 0, j = 8, k = 0;
            byte mask = 1;
            vec[0] ^= vec[0];
            while (i < len)
            {
                if (j > 0)
                {
                    if (str[i] != '0')
                        vec[k] |= mask;
                    mask <<= 1;
                    j--;
                    i++;
                }
                else
                {
                    j = 8;
                    k++;
                    vec[k] ^= vec[k];
                    mask = 1;
                }
            }
        }
        else
            throw 0;
    }
    else
        throw 1;

}

void BBV::Set0(int k)
{
    if (vec != NULL && k >= 0 && k < len)//проверяем существование вектора и корректность индекса
    {
        byte mask = 1;
        int j = k / 8;//номер нужной ячейки
        if (k >= 7)
            k %= 8;
        mask <<= k;//устанавливаем маску и записываем значение бита
        vec[j] &= ~mask;
    }
    else
        throw 1;

}

void BBV::Set1(int k)
{
    if (vec != NULL && k >= 0 && k < len)//проверяем существование вектора и корректность индекса
    {
        byte mask = 1;
        int j = k / 8;//номер нужной ячейки
        if (k >= 7)
            k %= 8;
        mask <<= k;//устанавливаем маску и записываем значение бита
        vec[j] |= mask;
    }
    else
        throw 1;
}

BBV BBV::operator=(BBV & V)
{
    if (this != &V)//проверка на самоприсвоение
    {
        len = V.len;//переписываем значения размеров и удаляем существующий вектор
        size = V.size;
        if (vec != NULL)
            delete vec;
        vec = new byte[size];//захватываем память под новый вектор и переписывем туда переданный вектор
        if (vec != NULL)
        {
            for (int i = 0; i < size; i++)
                vec[i] = V.vec[i];
        }
        else
            throw 0;
    }
    return *this;
}

BBV BBV::operator=(const char * str)
{
    if (str != NULL)//проверяем существует ли строка
    {
        len = strlen(str);//вычисляем размеры для вектора и удаляем старый вектор
        size = (len - 1) / 8 + 1;
        if (vec != NULL)
            delete vec;
        vec = new byte[size];//захватываем память для нового вектора и переписываем в его биты соотв. знач. строки
        if (vec != NULL)
        {
            int i = 0, j = 8, k = 0;
            byte mask = 1;
            vec[0] ^= vec[0];
            while (i < len)
            {
                if (j > 0)
                {
                    if (str[i] != '0')
                        vec[k] |= mask;
                    mask <<= 1;
                    j--;
                    i++;
                }
                else
                {
                    j = 8;
                    k++;
                    vec[k] ^= vec[k];
                    mask = 1;
                }
            }
        }
        else
            throw 0;
    }
    else
        throw 1;
    return *this;
}

bool BBV::operator==(BBV & V)
{
    if (vec != NULL && V.vec != NULL && V.len == len)//проверка существования векторов и равенство их длин
    {
        for (int i = 0; i < size; i++)//проверка совпадения ячеек
            if (V.vec[i] != vec[i])
                return false;
        return true;
    }
    return false;
}

BBV BBV::operator|(BBV & V)
{
    if (vec != NULL && V.vec != NULL && len == V.len)//проверка существования векторов и равенство их длин
    {
        BBV res(*this);//создаем результирующий вектор (чтобы не портить исходные)
        for (int i = 0; i < size; i++)//записываем в него значения сумм
            res.vec[i] = vec[i] | V.vec[i];
        return res;
    }
    else
        throw 2;
}

BBV BBV::operator&(BBV & V)
{
    if (vec != NULL && V.vec != NULL && len == V.len)//проверка существования векторов и равенство их длин
    {
        BBV res(*this);//создаем результирующий вектор (чтобы не портить исходные)
        for (int i = 0; i < size; i++)//записываем в него значения произведений
            res.vec[i] = vec[i] & V.vec[i];
        return res;
    }
    else
        throw 2;
}

BBV BBV::operator^(BBV & V)
{
    if (vec != NULL && V.vec != NULL && len == V.len)//проверка существования векторов и равенство их длин
    {
        BBV res(*this);//создаем результирующий вектор (чтобы не портить исходные)
        for (int i = 0; i < size; i++)//записываем в него значения операции
            res.vec[i] = vec[i] ^ V.vec[i];
        return res;
    }
    else
        throw 2;
}

BBV BBV::operator~()
{
    BBV res(*this);//создаем результирующий вектор
    if (vec != NULL)
    {
        for (int i = 0; i < size; i++)//делаем инверсию каждой ячейки
            res.vec[i] = ~vec[i];
        int k = len % 8;
        if (k != 0)
            k = 8 - k;
        res.vec[size - 1] <<= k;//чистим хвост в последней ячейке
        res.vec[size - 1] >>= k;
    }
    return res;
}

BBV BBV::operator>>(int k)
{
    BBV res(*this);//создаем результирующий вектор
    if (vec != NULL && k > 0)
    {
        for (int i = 0; i < size; i++)//обнуляем все ячейки результирующего вектора
            res.vec[i] ^= res.vec[i];
        int start = (k - 1) / 8;// первая ячейка для записи в векторе res
        if (k % 8 == 0)
            start = k / 8;
        k %= 8;//сдвиг в каждой ячейке
        byte mask = 0;
        for (int i = 0; start + i < size; i++)
        {
            res.vec[start + i] = vec[i] << k;//записываем сдвинутую ячейку
            res.vec[start + i] |= mask >> (8 - k);//дописали перенесенную часть, которая вышла за пределы ячейки
            mask = -1;
            mask <<= 8 - k;
            mask = vec[i] & mask;//запомнили часть для переноса
        }
        //чистим хвост
        if (k % 8)
        {
            int k = (8 - len % 8);
            res.vec[size - 1] <<= k;
            res.vec[size - 1] >>= k;
        }
    }
    return res;
}

BBV BBV::operator<<(int k)
{
    BBV res(*this);//создаем результирующий вектор
    if (vec != NULL && k > 0)
    {
        for (int i = 0; i < size; i++)//обнуляем все ячейки результирующего вектора
            res.vec[i] ^= res.vec[i];
        int start = size - 1 - (k - 1) / 8;// первая ячейка для записи в векторе res
        if (k % 8 == 0)
            start = size - 1 - k / 8;
        k %= 8;//сдвиг в каждой ячейке
        byte mask = 0;
        for (int i = 0; start - i >= 0; i++)
        {
            res.vec[start - i] = vec[size - 1 - i] >> k;//записываем сдвинутую ячейку, которая вышла за пределы ячейки
            res.vec[start - i] |= mask << (8 - k);//дописали перенесенную часть
            mask = -1;
            mask >>= 8 - k;
            mask = vec[size - 1 - i] & mask;//запомнили часть для переноса
        }
    }
    return res;
}

X BBV::operator[](int k)
{
    if (vec != NULL && k >= 0 && k < len)
    {
        X ptr(&(vec[k / 8]), k % 8);//формируем объект типа X, куда помещаем указатель нужной ячейки и номер бита
        return ptr;
    }
    else
        throw 1;
}

BBV::operator char*()
{
    if (vec != NULL)//проверяем существование вектора
    {
        char* str = new char[len + 1];//захватываем память под строку
        if (str != NULL)
        {
            int k = 0;
            byte mask = 1;
            for (int i = 0; i < len; i++)//переписываем значения из битов в ячейки строки
            {
                if (i % 8 == 0 && i != 0)
                {
                    k++;
                    mask = 1;
                }
                if ((vec[k])&(mask))
                {
                    str[i] = '1';
                }
                else
                    str[i] = '0';
                mask <<= 1;
            }
            str[len] = '\0';
        }
        else
            throw 0;
        return str;
    }
    return NULL;
}

int BBV::getWeight()
{
    if (vec != NULL)
    {
        int count = 0;
        for (int i = 0; i < size; i++)
        {
            byte v = vec[i];
            while (v != 0)
            {
                count++;
                v &= (v - 1);
            }
        }
        return count;
    }
    return 0;

}

int BBV::getSize()
{
    return len;
}

X::X()
{
    ptr = NULL;
    index = 0;
}

X::X(byte* vec, int k)//передаем в конструктор указатель на нужную ячейку и номер бита в ней
{
    if (vec != NULL && k >= 0 && k <= 7)//проверяем допустимость номера бита и существование ячейки
    {
        ptr = vec;
        index = k;
    }
    else
        throw 1;
}

X X::operator=(int k)
{
    if (ptr != NULL)//проверяем существует ли ячейка с битом, в который будет производится запись значения
    {
        byte mask = 1;//подготавливаем маску с правильно размещенным битом со значение 1
        mask <<= index;
        if (k == 0)//производим запись в ячейку
            *ptr &= ~mask;
        else
            *ptr |= mask;
    }
    else
        throw 1;
    return *this;
}
X::operator int()
{
    if (ptr != NULL)//проверяем сущестование ячейки с нужным битом
    {
        int k = 0;//полагаем что нужный бит равен 0
        byte mask = 1;//формируем маску с выставленной 1 в нужном бите
        mask <<= index;
        if (*ptr & mask)//если предположение раввенства 0 не верно, запишем в k новое значение 1
            k = 1;
        return k;
    }
    else
        throw 1;
}

X X::operator=(X& v)
{
    if (ptr != NULL && v.ptr != NULL)
    {
        byte mask = 1;//подготавливаем маску с правильно размещенным битом со значение 1
        mask <<= v.index;
        if (mask & *(v.ptr))//производим запись в ячейку в нужный бит
            *this = 1;
        else
            *this = 0;
    }
    else
        throw 1;
}

ostream & operator << (ostream &r, BBV& V)
{
    if (V.vec != NULL)//првоверяем существование ячеек булева вектора
    {
        int i = 0, j = 8, k = 0;
        byte mask = 1;
        while (i < V.len)//цикл для прохода по всем битам вектора
        {
            if (j > 0)//в каждую ячейку заходим 8 раз. счетчик j отвечает за количество заходов в ячейку
            {
                if (V.vec[k] & mask)//с помощью маски получаем значение бита и выводим его
                    cout << '1';
                else
                    cout << '0';
                mask <<= 1;
                j--;
                i++;
            }
            else//после полного прохода по ячейке переходим к следующей
            {
                j = 8;
                k++;
                mask = 1;
            }
        }
        cout << '\n';
    }
    return r;
}
istream & operator >> (istream &r, BBV& V)// return r;
{
    int str_size;//запрашиваем размер вектора
    cout << "Input size for BBV:\n";
    cin >> str_size;
    while (str_size < 0)//исключаем случай отрицательного размера вектора
    {
        cout << "Re-enter size for BBV:\n";
        cin >> str_size;
    }
    char* str = new char[str_size + 1];//создаем строку для вектора у учетом символа \0
    if (str != NULL)//если память выделилась, то вводим в строку и инициализируем ей вектор
    {
        cout << "Input BBV:\n";
        cin >> str;
        V.Init(str);
        V.len = str_size;
        V.size = (str_size - 1) / 8 + 1;
        delete[]str;
    }
    else
        throw 0;
    return r;
}
