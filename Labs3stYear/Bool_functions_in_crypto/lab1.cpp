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
    BF(const BF& in_other);
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
    uint32_t * func;

    void initialize(int nn, const int type);
    uint32_t generateRandomFunc();
};

BF::BF(int nn, const int type) {
    initialize(nn, type);
}

BF::BF(const char* str) {
    int len = strlen(str);

    if (len & (len - 1)) {
        std::puts("Error: Length must be a power of 2.");
        std::exit(1);
    }

    n = len;
    nw = len / 32 + (len % 32 != 0);
    func = new uint32_t[nw];

    int index = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == '1') {
            func[index / 32] |= 1 << (index % 32);
        }
        ++index;
    }

    // Fill remaining bits with zeros
    for (int i = index; i < nw * 32; ++i) {
        func[i / 32] &= ~(1 << (i % 32));
    }
}

void BF::initialize(int nn, const int type) {
    n = nn;
    //nw = n / 8 + bool(n % 8);

    nw = ((1 << n) + 31) >> 5;
    func = new uint32_t[nw];

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
        int bits = 1 << n; //int bits = pow(2, n);
        uint32_t mask = 0;
        mask = ~mask;
        int count = 0;
        for (; bits >= 32; bits -= 32) 
        {
            func[count++] = mask;
        }
        mask = (1 << bits) - 1; //mask = bits;  //fix this method
        func[count] = mask;
        
    }
    else if (type == 2) {
        int bits = 1 << n;  
        //int bits = pow(2, n);
        int count = 0;
        for (; bits > 32; bits -= 32)
        {
            func[count++] = generateRandomFunc(); //fix this construction
        }
        func[count] = generateRandomFunc();
        /*  
        uint32_t mask = 0;
        for (int i = 0; i < n % 32; ++i) {
            mask |= 1 << i;
        }
        func[nw - 1] = generateRandomFunc();
        func[nw - 1] &= mask;
        */
    }
    else {
        std::cerr << "Invalid type.\n";
        //delete[] func;
        std::exit(1);
    }
}

uint32_t BF::generateRandomFunc() {
    uint32_t mask = rand() - rand(); // Added initialization
    return mask; // Added return statement
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

    for (int i = 0; i < nw; ++i) {
        func[i] = in_other.func[i];
    }
}

int main() {
    srand(time(0));

    BF b1(2, 2); // Создаем объект с 8 битами и случайными значениями
    std::cout << "Weight of b1: " << b1.weigth() << std::endl;
    std::cout << "b1: " << b1 << std::endl;

    BF b2("10111011000000000000000000000000"); // Создаем объект из строки
    std::cout << "Weight of b2: " << b2.weigth() << std::endl;
    std::cout << "b2: " << b2 << std::endl;


    return 0;
}
