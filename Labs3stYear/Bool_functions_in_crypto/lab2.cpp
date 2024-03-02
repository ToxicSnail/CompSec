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
    BF& operator=(const BF& V);
    ~BF()
    {
        if (func) 
        {
            delete[] func;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const BF& bf);
    friend std::istream& operator>>(std::istream& is, BF& bf);

    int weigth(); //подсчет веса вектора с помощью отнятия единицы и конъюнкции с оригиналом
    int get_n();
    void test_generate_weight();
    BF mobius_transform();
    bool isEqual(const BF& other) const;
    void buildANF();
    void test_generate_mobius_transform();

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

size_t count_zeros(unsigned int len)
{
    size_t count = 0;
    unsigned int mask = 0;
    for (int i = 0; i < 31; i++)
    {
        count++;
        mask = 1U << i;
        if (len & mask)
        {
            break;
        }
    }
    return count - 1;
}

BF& BF::operator=(const BF& V)
{
    if (this != &V) {
        delete[] func;
        n = V.n;
        nw = V.nw;
        func = new uint32_t[nw];
        for (int i = 0; i < nw; ++i) {
            func[i] = V.func[i];
        }
    }
    return *this;
}

BF::BF(const char* str) 
{
    int len = strlen(str);

    if (len & (len - 1)) 
    {
        std::puts("Error: Length must be a power of 2.");
        std::exit(1);
    }

    n = count_zeros(len);
    //n = len;
    nw = len / 32 + (len % 32 != 0);
    func = new uint32_t[nw];

    uint32_t mask = 0;
    for (int i = 0; i < nw; ++i)
    {
        func[i] = mask;     //test this fragment code
    }

    int index = 0;
    for (int i = 0; i <= len-1; ++i) 
    {
        if (str[i] == '1') {
            func[index / 32] |= 1 << (index % 32);
        }
        ++index;
    }

    // fill bits with zeros
    //for (int i = index; i < nw * 32; ++i) {
        //func[i / 32] &= ~(1 << (i % 32));
    //}
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

std::ostream& operator<<(std::ostream& out, const BF& f)
{
    if (f.func)
    {
        unsigned int mask;
        for (size_t ix_byte = 0; ix_byte < f.nw; ix_byte++) // цикл по байтам
        {
            // цикл по битам, есть возможность вывода байта rtl и ltr
            if (f.n < 5)
            {
                int counterOfBits = 0;
                int bits = 1 << f.n;
                for (int ix_bit_ltr = 0, ix_bit_rtl = 31; ix_bit_rtl >= 0 && ix_bit_ltr < 32; ix_bit_ltr++, ix_bit_rtl--)
                {
                    mask = 1U << ix_bit_ltr; // сдвигаем маску на нужное количесто бит
                    // с помощью & проверяем установлен бит или нет, если нет то выводим 0, если да то выводим 1
                    out << ((f.func[ix_byte] & mask) ? 1 : 0);
                    counterOfBits++;
                    if (counterOfBits == bits)
                        break;
                }
            }
            else
            {
                for (int ix_bit_ltr = 0, ix_bit_rtl = 31; ix_bit_rtl >= 0 && ix_bit_ltr < 32; ix_bit_ltr++, ix_bit_rtl--)
                {
                    mask = 1U << ix_bit_ltr; // сдвигаем маску на нужное количесто бит
                    // с помощью & проверяем установлен бит или нет, если нет то выводим 0, если да то выводим 1
                    std::cout << ((f.func[ix_byte] & mask) ? 1 : 0);
                }
                out << " "; // после каждого байта выводим пробел для формата
            }
        }
    }
    else
        out << "NULLptr"; // если указатель пуст

    return out;
}



std::istream& operator>>(std::istream& is, BF& bf)
{
    std::string input;
    is >> input;
    bf = BF(input.c_str());
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

BF BF::mobius_transform() {
    BF result(*this);

    unsigned int NW;
    if (n < 5)
        NW = 1;
    else
        NW = (unsigned int)(1 << (n - 5));

    unsigned int mask1 = 0xAAAAAAAA;
    unsigned int mask2 = 0xCCCCCCCC;
    unsigned int mask3 = 0xF0F0F0F0;
    unsigned int mask4 = 0xFF00FF00;
    unsigned int mask5 = 0xFFFF0000;
    
    for (int i = 0; i < NW; ++i) {
        unsigned int g = result.func[i];
        g = g ^ ((g << 1) & mask1);
        g = g ^ ((g << 2) & mask2);
        g = g ^ ((g << 4) & mask3);
        g = g ^ ((g << 8) & mask4);
        g = g ^ ((g << 16) & mask5);
        result.func[i] = g;
    }

    if (n < 5) {
        //g.func[0] &= (1u << (1 << n)) - 1u; //обнуляем незначащие биты
        int bits = 1 << n;
        result.func[0] <<= 32 - bits;
        result.func[0] >>= 32 - bits;
    }
    else if (n > 5)
    {
        for (int k = 1; k < NW; k <<= 1) {
            for (int j = 0; j < NW; j += k << 1) {
                for (int s = j; s < j + k; s++) {
                    result.func[s + k] ^= result.func[s];
                }
            }
        }
    }
    return result;

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

void BF::buildANF() 
{
    bool isFirstTerm = true;
    for (int i = 0; i < nw; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (func[i] & (1 << j)) {
                int var_index = i * 32 + j;
                if (!isFirstTerm)
                    std::cout << "  +  ";
                isFirstTerm = false;
                //std::cout << "1";
                for (int k = 0; k < n; ++k) {
                    if (var_index & (1 << k))
                        std::cout << " x_" << (k+1);
                    //else
                        //std::cout << "! x_" << (k+1);
                }
            }
        }
    }
    std::cout << std::endl;
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

void test_generate_mobius_transform()
{
    for (int i = 0; i < 32; i++)
    {
        BF randomBF1(i, 2);
        BF randomBF2 = randomBF1.mobius_transform();
        BF randomBF3 = randomBF2.mobius_transform();
        
        std::cout << "Round " << i << " ";
        std::cout << "    b and b1 are equal: " << (randomBF1.isEqual(randomBF3) ? "true" : "false") << std::endl;
    }
}

int main() 
{
    srand(time(NULL));
    
    //test_generate_weight();     //тест на создание случайного вектора с высчитыванием веса
    
    BF b("0"); // Создание объекта с 32 битами и случайными значениями
    //BF b2("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    BF b_transformed = b.mobius_transform();
    std::cout << b_transformed << std::endl;

    b_transformed.buildANF();
    std::cout << "ANF of b after Mobius transform: ";

    /*
    BF b_input;
    std::cin >> b_input;
    BF b_transformed = b_input.mobius_transform();
    std::cout << "Resulting BF object: " << b_transformed << std::endl;

    /*
    BF b(3,2);
    std::cin >> b;
    std::cout << std::endl << b << std::endl;
    BF b_transformed = b.mobius_transform();
    std::cout << b_transformed << std::endl;

    /*
    BF b1(b);
    BF b2("11111111111111111111111111111111");
    b2.mobius_transform();
    std::cout << "b2 before Mobius transform : " << b2 << std::endl;

    
    std::cout << "b before Mobius transform : " << b << std::endl;
    b1.mobius_transform();
    std::cout << "b1 after 1 Mobius transform : " << b1 << std::endl;
    b1.mobius_transform();
    std::cout << "b1 after 2 Mobius transforms: " << b1 << std::endl;
    std::cout << "b and b1 are equal: " << (b.isEqual(b1) ? "true" : "false") << std::endl;     // Проверка на равенство
    
    //test_generate_mobius_transform();

    //b.buildANF();
    //std::cout << "ANF of b: " << b << std::endl;

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
