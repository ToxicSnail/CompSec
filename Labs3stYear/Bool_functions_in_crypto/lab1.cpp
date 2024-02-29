#include <iostream>
#include <cstdlib>
#include <climits>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <cstring>

class BF
{
public:
    BF(int nn = 1, const int type = 0);    //type = {0,1,2}, if type = 0, then f = 0. type =1, f =1. type 2 - random  //может быть конструктором по умолчанию
    BF(const char* str); //может быть не 2**n длина, нужен контроль
    BF(const BF& in_other);
    ~BF()
    {
        if (func) 
        {
            delete[] func;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const BF& bf);
    friend std::istream& operator>>(std::istream& is, BF& bf);
    BF operator=(const BF& V);

    int weigth(); //подсчет веса вектора с помощью отнятия единицы и конъюнкции с оригиналом
    int get_n();
    void test_generate_weight();
    void mobius_transform();
    bool isEqual(const BF& other) const;

private:
    int n, nw;  //nw - колво байт, n - бит
    unsigned int* func;

    void initialize(int nn, const int type);
    uint32_t generateRandomFunc();
};

BF::BF(int nn, const int type) 
{
    initialize(nn, type);
}

BF::BF(const char* str) 
{
    int len = strlen(str);

    if (len & (len - 1)) 
    {
        std::puts("Error: Length must be a power of 2.");
        std::exit(1);
    }

    //n = len;
    nw = len / 32 + (len % 32 != 0);
    func = new uint32_t[nw];

    int index = 0;
    for (int i = len - 1; i >= 0; --i) 
    {
        if (str[i] == '1') {
            func[index / 32] |= 1 << (index % 32);
        }
        ++index;
    }

    // fill bits with zeros
    for (int i = index; i < nw * 32; ++i) {
        func[i / 32] &= ~(1 << (i % 32));
    }
}

void BF::initialize(int nn, const int type) 
{
    n = nn;
    //nw = n / 8 + bool(n % 8);

    nw = ((unsigned int)(1 << n) + 31) >> 5;  //[2**n / 32]
    func = new uint32_t[nw];

    if (type == 0) 
    {
        uint32_t mask = 0;
        for (int i = 0; i < nw; ++i) 
        {
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
        for (count = 0; count < nw; ++count)
        {
            func[count++] = mask;
        }
        mask = (1 << bits) - 1; //mask = bits;  //fix this method
        func[count] = mask;

    }
    else if (type == 2) 
    {
        int bits = 1 << n;
        //int bits = pow(2, n);
        int count = 0;
        for (count = 0; count < nw; ++count)
        {
            func[count] = generateRandomFunc(); //fix this construction
        }
        if (n < 5)
        {
            func[nw - 1] = generateRandomFunc() << (32 - bits);
            func[nw - 1] >>= 32 - bits;
        }



        // Генерация случайных битов в последнем слове
        //func[nw - 1] = generateRandomFunc();

        // Сдвиг влево, чтобы оставить только 2^n бит
        //func[count] <<= (32 - bits);

        // Сдвиг обратно вправо, чтобы удалить незначащие биты
        //func[count] >>= (32 - bits);

        // Заполнение остальных слов нулями
        //for (int i = 0; i < nw - 1; ++i) {
        //    func[i] = 0;
        //}

    }
    else 
    {
        std::cerr << "Invalid type.\n";
        //delete[] func;
        std::exit(1);
    }
}

uint32_t BF::generateRandomFunc() 
{
    unsigned int mask = rand() - rand(); // Added initialization
    return mask; // Added return statement
}

int BF::weigth() 
{
    int weight = 0;
    for (int i = 0; i < nw; ++i) 
    {
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
    bool significant_bit_found = false; // флаг для определения, был ли найден значимый бит

    for (int i = in_bfunc.nw - 1; i >= 0; --i) 
    { // начинаем с последнего слова
        for (int n = 31; n >= 0; --n) 
        { // начинаем с младшего бита в слове
            if (((1 << n) & in_bfunc.func[i]) || significant_bit_found) 
            {
                in_out << (((1 << n) & in_bfunc.func[i]) ? '1' : '0');
                significant_bit_found = true; // устанавливаем флаг, если найден значимый бит
            }
        }
        if (significant_bit_found || i == 0) // выводим пробел только если найден значимый бит или это последнее слово
            in_out << " ";
    }

    return in_out;
}

std::istream& operator>>(std::istream& is, BF& bf) 
{
    char buffer;
    int index = 0;
    while (is >> buffer) 
    {
        if (buffer == '0') 
        {
            bf.func[index / 32] &= ~(1 << (index % 32));
        }
        else if (buffer == '1') 
        {
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

BF::BF(const BF& in_other) 
{    
    n = in_other.n;
    uint32_t bits = 1 << n; //pow(2,n);
    nw = in_other.nw;
    func = new uint32_t[nw]();

    for (int i = 0; i < nw; ++i) 
    {
        func[i] = in_other.func[i];
    }
}

int BF::get_n()
{
    return n;
}

void BF::mobius_transform()
{
    uint32_t mask1 = 0xAA;
    uint32_t mask2 = 0xCC;
    uint32_t mask3 = 0xF0;
    uint32_t mask4 = 0xFF;
    uint32_t mask5 = 0xFFFF;

    int bits = 1 << n;

    for (int i = 0; i < nw; ++i) 
    {
        uint32_t g = func[i];
        g = g ^ ((g << 1) & mask1);
        g = g ^ ((g << 2) & mask2);
        g = g ^ ((g << 4) & mask3);
        g = g ^ ((g << 8) & mask4);
        g = g ^ ((g << 16) & mask5);
        func[i] = g;
    }

    func[nw - 1] <<= 32 - bits;
    func[nw - 1] >>= 32 - bits;
}

bool BF::isEqual(const BF& other) const 
{
    if (n != other.n)
        return false;

    for (int i = 0; i < nw; ++i) 
    {
        if (func[i] != other.func[i])
            return false;
    }

    return true;
}

void test_generate_weight()
{
    BF b1(3, 2); // Создаем объект с 2**n битами и значениями от параметра type
    std::cout << "Weight of b1: " << b1.weigth() << std::endl;
    std::cout << "b1: " << b1 << std::endl;
    std::cout << "Weight(f)/2**n: " << ((float)b1.weigth() / (1 << b1.get_n())) << std::endl << std::endl;

    for (int i = 0; i < 32; i++)
    {
        BF random(i, 2);
        std::cout << "Round " << i << " ";
        std::cout << (double)(random.weigth()) / (unsigned int)(1 << random.get_n()) << std::endl;
    }
}

int main() 
{
    srand(time(NULL));
    
    //test_generate_weight();     //тест на создание случайного вектора с высчитыванием веса

    BF b(7, 2); // Создание объекта с 32 битами и случайными значениями
    BF b1(b);

    std::cout << "b before Mobius transform : " << b << std::endl;

    b1.mobius_transform();
    std::cout << "b1 after 1 Mobius transform : " << b1 << std::endl;
    b1.mobius_transform();
    std::cout << "b1 after 2 Mobius transforms: " << b1 << std::endl;

    std::cout << "b and b1 are equal: " << (b.isEqual(b1) ? "true" : "false") << std::endl;     // Проверка на равенство

    return 0;
    /*
    BF b2("11"); // Создаем объект из строки
    std::cout << std::endl << "Weight of b2: " << b2.weigth() << std::endl;
    std::cout << "b2: " << b2 << std::endl << std::endl;

    BF b3(b2);
    std::cout << "Weight of b3: " << b3.weigth() << std::endl;
    std::cout << "b3: " << b3 << std::endl;
    */


    return 0;
}
