// Online C++ compiler to run C++ program online
#include <iostream>

class BF
{
public:
    BF(int nn = 1, const int type = 0);    //type = {0,1,2}, if type = 0, then f = 0. type =1, f =1. type 2 - random  //может быть конструктором по умолчанию
    BF(char* str); //может быть не 2**n длина, нужен контроль
        //конструкторы-деструкторы
        //вывод
        //ввод и вывод согласованы
    int weigth(); //подсчет веса вектора с помощью отнятия единицы и конъюнкции с оригиналом
        
    ~BF()
    {
        if (func){
            delete [] func;
        }
    }
    
        

private:
    int n, nw;  //nw - колво байт, n - бит
    unsigned int *func;
}

BF::BF(int nn, const int type)
{
    if type == 2
    {
        
    }
    else if type == 1
    {
        
    }
    else
    {
        n = nn;
        func = 0;
    }
}

BF::BF(char* str)
{
    str_len = strlen(str);
    if (str_len & (str_len - 1)) != 0)
    {
        puts("Error!");
        exit(1);
    }
}

int main() {
    BoolV vec1(10);

    return 0;
}
