#include <iostream>
#include <cstdlib>
#include <climits>
#include <stdio.h>
#include <cmath>

class BF
{
public:
    BF(int nn = 1, const int type = 0);    //type = {0,1,2}, if type = 0, then f = 0. type =1, f =1. type 2 - random  //может быть конструктором по умолчанию
    BF(const char* str); //может быть не 2**n длина, нужен контроль
    ~BF()
    {
        if (func) {
            delete[] func;
        }
    }

    int weigth(); //подсчет веса вектора с помощью отнятия единицы и конъюнкции с оригиналом

    friend std::ostream& operator<<(std::ostream& os, const BF& bf);
    friend std::istream& operator>>(std::istream& is, BF& bf);
    BF operator=(const BF& V);

private:
    int n, nw;  //nw - колво байт, n - бит
    unsigned int* func;

    void initialize(int nn, const int type);
    uint32_t generateRandomFunc();
};

BF::BF(int nn, const int type) {
    initialize(nn, type);
}

BF::BF(const char* str) {
    // подсчет длины строки
    int len = 0;
    while (str[len] != '\0') {  //"10101001/n"
        ++len;      //переделать без счетчика
    }

    // Проверка равенства длины степени
    if (len & (len - 1)) {
        std::puts("Error");
        std::exit(1);
    }

    // Инициализация переменных
    n = len;
    nw = ((1 << n) + 31) >> 5;
    func = new unsigned int[nw];

    // Копирование битов из строки
    int index = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == '0') {
            func[index / 32] &= ~(1 << (index % 32));
        }
        else if (str[i] == '1') {
            func[index / 32] |= 1 << (index % 32);
        }
        else {
            std::cerr << "Invalid character in input string.\n";
            delete[] func;
            std::exit(1);
        }
        ++index;
    }
}

void BF::initialize(int nn, const int type) {
    n = nn;
    //nw = n / 8 + bool(n % 8);
    if (n <= 5)
    {
        nw = 1;
    }
    else if (n > 5)
    {
        nw = pow(2,n - 1);
    }
    //nw = ((1 << n) + 31) >> 5;
    func = new unsigned int[nw];

    if (type == 0) {
        uint32_t mask = 0;
        for (int i = 0; i < nw; ++i) {
            func[i] = mask;     //test this fragment code
        }
    }
    else if (type == 1) {
        /*for (int i = 0; i < nw; ++i) {
            func[i] = UINT_MAX;
        }*/
        int bits = pow(2, n);
        uint32_t mask = 0;
        mask = ~mask;
        int count = 0;
        for (; bits > 32, bits = bits - 32;)
        {
            func[count] = mask;
        }
        mask = (uint32_t)bits;  //fix this method
        func[count] = mask;
        
    }
    else if (type == 2) {
        int bits = pow(2, n);
        int count = 0;
        for (; bits > 32, bits = bits - 32;)
        {
            (uint32_t)func[count] = generateRandomFunc; //fix this construction
        }
        (uint32_t)func[count] = generateRandomFunc();   //сместить и вернуть, чтобы незнач нули появились вместо случайных чисел
    }
    else {
        std::cerr << "Invalid type.\n";
        delete[] func;
        std::exit(1);
    }
}

uint32_t BF::generateRandomFunc() {
    for (int i = 0; i < nw; ++i) {
        uint32_t mask = rand();
        func[i] = mask;
    }
}

int BF::weigth() {
    int weight = 0;
    for (int i = 0; i < nw; ++i) {
        unsigned int temp = func[i];
        while (temp != 0) {
            temp &= temp - 1;
            ++weight;
        }
    }
    return weight;
}

std::ostream& operator<<(std::ostream& in_out, const BF& in_bfunc)
{
    for (uint32_t i = 0; i < in_bfunc.nw; ++i)
    {
        for (uint32_t n = 0; n < 32; ++n) {
            in_out << (((1 << n) & in_bfunc.func[i]) ? '1' : '0');
        }
        in_out << " ";
    }

    return in_out;
}

std::istream& operator>>(std::istream& is, BF& bf) {
    char buffer;
    int index = 0;
    while (is >> buffer) {
        if (buffer == '0') {
            bf.func[index / 32] &= ~(1 << (index % 32));
        }
        else if (buffer == '1') {
            bf.func[index / 32] |= 1 << (index % 32);
        }
        else {
            is.setstate(std::ios::failbit);
            return is;
        }
        ++index;
    }
    return is;
}

BF::BF(const BF& in_other) {    //test this fragment code
    n = in_other.n;
    uint32_t bits = pow(n, 2);
    nw = in_other.nw;
    func = new uint32_t[nw]();

    for (uint32_t i = 0; i < nw; ++i) {
        func[i] = in_other.func[i];
    }
}

int main() {
    srand(time(0));

    BF b1(2, 1); // Создаем объект с 8 битами и случайными значениями
    std::cout << "Weight of b1: " << b1.weigth() << std::endl;
    std::cout << "b1: " << b1 << std::endl;

    BF b2("10101010"); // Создаем объект из строки
    std::cout << "Weight of b2: " << b2.weigth() << std::endl;
    std::cout << "b2: " << b2 << std::endl;


    return 0;
}
