#include <iostream>
#include <cstdlib>
#include <climits>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <cstring>
#include <vector> 
#include <ctime>
#include <string>
#include <algorithm>
#include <cassert>  // Для assert()

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
    //void test_generate_weight();
    BF mobius_transform();
    bool is_equal(const BF& other) const;
    void build_anf();
    int deg();

    std::vector<int> transform_hadamard();
    int max_cor_imm();
    int hamming_weight(unsigned int value);

    int nonlinearity();
    void build_best_affine();

    void print_all_subfunctions(int m);

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
        func[i] = mask;
    }

    int index = 0;
    for (int i = 0; i <= len - 1; ++i)
    {
        if (str[i] == '1') {
            func[index / 32] |= 1 << (index % 32);
        }
        ++index;
    }
}

void BF::initialize(int nn, const int type)
{
    n = nn;

    nw = ((unsigned int)(1 << n) + 31) >> 5;  //[2**n / 32]
    func = new uint32_t[nw];

    if (type == 0)
    {
        uint32_t mask = 0;
        for (int i = 0; i < nw; ++i)
        {
            func[i] = mask;
        }
    }
    else if (type == 1) {
        int bits = 1 << n; //int bits = pow(2, n);
        uint32_t mask = 0;
        mask = ~mask;
        int count = 0;
        for (count = 0; count < nw; ++count)
        {
            func[count++] = mask;
        }
        mask = (1 << bits) - 1;
        func[count] = mask;

    }
    else if (type == 2)
    {
        int bits = 1 << n;
        int count = 0;
        for (count = 0; count < nw; ++count)
        {
            func[count] = generateRandomFunc();
        }
        if (n < 5)
        {
            func[nw - 1] = generateRandomFunc() << (32 - bits);
            func[nw - 1] >>= 32 - bits;
        }
    }
    else
    {
        std::cerr << "Invalid type.\n";
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
        while (temp != 0)
        {
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
    uint32_t bits = 1 << n;
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

bool BF::is_equal(const BF& other) const
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

void BF::build_anf() {
    bool is_first_term = true;

    if (func[0] & 1) {
        std::cout << "1";
        is_first_term = false;
    }
    for (int i = 0; i < nw; ++i)
    {
        for (int j = 1; j < 32; ++j)
        {
            if (func[i] & (1 << j))
            {
                int var_index = (i << 5) + j;

                if (!is_first_term)
                {
                    std::cout << " + ";
                }
                for (int k = 0; k < n; ++k)
                {
                    if (var_index & (1 << k))
                        std::cout << "x_" << (k + 1);
                }
                is_first_term = false;
            }
        }
    }

    if (is_first_term)
    {
        std::cout << "0";
    }
    std::cout << std::endl;
}

int BF::deg()
{
    int max_degree = 0;

    for (int i = 0; i < nw; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            if (func[i] & (1 << j))
            {
                unsigned int var_index = (i << 5) + j;
                int degree = 0;
                while (var_index != 0) {
                    var_index &= var_index - 1;
                    ++degree;
                }
                if (degree > max_degree)
                    max_degree = degree;
            }
        }
    }

    return max_degree;
}


//void test_generate_weight()
//{
//    BF b1(5, 2); // Создаем объект с 2**n битами и значениями от параметра type
//    std::cout << "Weight of b1: " << b1.weigth() << std::endl;
//    std::cout << "b1: " << b1 << std::endl;
//    std::cout << "Weight(f)/2**n: " << ((float)b1.weigth() / (1 << b1.get_n())) << std::endl << std::endl;
//
//    for (int i = 0; i < 32; i++)
//    {
//        BF random(i, 2);
//        std::cout << "Round " << i << " ";
//        std::cout << (double)(random.weigth()) / (unsigned int)(1 << random.get_n()) << std::endl;
//    }
//}

void test_generate_mobius_transform()
{
    for (int i = 0; i < 32; i++)
    {
        BF randomBF1(i, 2);
        BF randomBF2 = randomBF1.mobius_transform();
        BF randomBF3 = randomBF2.mobius_transform();

        std::cout << "Round " << i << " ";
        std::cout << "    b and b1 are equal: " << (randomBF1.is_equal(randomBF3) ? "true" : "false") << std::endl;
    }
}


std::vector<int> BF::transform_hadamard()
{


    size_t size = 1U << n;
    std::vector<int> transformed_func(size, 0);

    for (int i = 0; i < nw; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            if (i * 32 + j < size)
            {
                if (func[i] & (1U << j))
                {
                    transformed_func[i * 32 + j] = -1;
                }
                else
                {
                    transformed_func[i * 32 + j] = 1;
                }
            }
        }
    }

    // Второй шаг: Выполняем бабочку 


    for (int length = 1; length < size; length *= 2)
    {
        for (int i = 0; i < size; i += 2 * length)
        {
            for (int j = 0; j < length; ++j)
            {
                int a = transformed_func[i + j];
                int b = transformed_func[i + j + length];
                transformed_func[i + j] = a + b;
                transformed_func[i + j + length] = a - b;
            }
        }
    }

    for (int value : transformed_func)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return transformed_func;
}

int BF::max_cor_imm()
{
    std::vector<int> walsh_vector = transform_hadamard();
    unsigned int a, b, c;

    for (int i = 1; i <= n; i++)
    {
        a = ((1 << i) - 1) << (n - i);

        if (walsh_vector[a] != 0)
        {
            return (i - 1);
        }
        std::cout << a << std::endl;
        while (a != ((1 << i) - 1))
        {
            b = (a + 1) & a;
            c = hamming_weight((b - 1) ^ a) - 2;

            a = (((((a + 1) ^ a) << 1) + 1) << c) ^ b;

            std::cout << a << std::endl;

            if ((walsh_vector[a] != 0))
            {
                return i - 1;
            }
        }
    }

    return n;
}

int BF::hamming_weight(unsigned int value) {
    int weight = 0;
    while (value) {
        weight += value & 1;
        value >>= 1;
    }
    return weight;
}

int BF::nonlinearity()
{
    int max_in_f = 0;
    std::vector<int> walsh_vector = transform_hadamard();

    for (size_t i = 0; i < walsh_vector.size(); ++i)
    {
        int iter = walsh_vector[i];
        if (abs(iter) > max_in_f)
            max_in_f = abs(iter);
    }

    return (1 << (n - 1)) - (max_in_f / 2);
}

void BF::build_best_affine()
{
    std::vector<int> walsh_vector = transform_hadamard();
    bool is_first_term = true;
    int max_walsh_value = 0;
    int max_walsh_index = 0;
    int weight_t;

    for (int i = 0; i < walsh_vector.size(); ++i)
    {
        if (std::abs(walsh_vector[i]) > max_walsh_value)
        {
            max_walsh_value = std::abs(walsh_vector[i]);
            max_walsh_index = i;
        }
    }


    std::cout << "Max value in walsh_vector: " << max_walsh_value << std::endl;


    if (max_walsh_index == 0 && walsh_vector[0] > 0)   std::cout << 0;
    else if (walsh_vector[max_walsh_index] < 0)
    {

        weight_t = hamming_weight(max_walsh_index);
        for (unsigned int ix = 0, mask = 1; ix < n; mask <<= 1, ix++)
        {
            if (max_walsh_index & mask)
            {
                weight_t--;
                if (weight_t > 0)
                {
                    std::cout << "x_" << n - ix << " + ";
                    is_first_term = false;
                }
                else
                {
                    std::cout << "x_" << n - ix;
                    is_first_term = false;
                }
            }
        }
        if (is_first_term)
            std::cout << "1";
        else
        {
            std::cout << " + 1";
        }
        std::cout << std::endl;

    }
    else
    {
        weight_t = hamming_weight(max_walsh_index);
        for (unsigned int ix = 0, mask = 1; ix < n; mask <<= 1, ix++)
        {
            weight_t--;
            if (weight_t > 0)
            {
                std::cout << "x_" << n - ix << " + ";
            }
            else
            {
                std::cout << "x_" << n - ix;
            }
        }
        std::cout << " + 1" << std::endl;
    }
}


void BF::print_all_subfunctions(int m)
{
    int n = get_n();
    unsigned int NW = (n < 5) ? 1 : (1u << (n - 5));

    unsigned int* func_copy = new unsigned int[NW];
    std::memcpy(func_copy, func, sizeof(unsigned int) * NW);

    // Фиксация каждой переменной
    for (int j = 0; j < n; ++j) {
        // Новое количество переменных и размер массива после фиксации
        int new_n = n - 1;
        unsigned int new_NW = (new_n < 5) ? 1 : (1u << (new_n - 5));

        unsigned int* temp_func = new unsigned int[new_NW]();

        for (int fix = 0; fix <= 1; ++fix) {
            // Пересчитываем функцию с учетом фиксации переменной x_j
            for (unsigned int i = 0; i < NW; ++i) {
                unsigned int new_index = i & ~(1u << j);  // Убираем j-ую переменную из индекса
                new_index = (i > j) ? (new_index >> 1) : new_index;  // Сдвигаем биты правее j на один влево

                if (fix == 0 && (i & (1 << j)) != 0)
                    continue;  // Игнорируем мономы с x_j, если x_j фиксирована в 0

                temp_func[new_index] |= func_copy[i];
            }

            // Вывод измененной функции
            std::cout << "After fixing x_" << j << " to " << fix << ": ";
            for (unsigned int i = 0; i < new_NW; ++i) {
                for (int bit = 0; bit < (1 << new_n); bit++) {
                    std::cout << ((temp_func[i] >> bit) & 1);
                }
                std::cout << " ";
            }
            std::cout << std::endl;
            std::memset(temp_func, 0, sizeof(unsigned int) * new_NW);  // Очистка для следующей фиксации
        }

        // Обновляем исходные данные для следующей переменной
        std::memcpy(func_copy, temp_func, sizeof(unsigned int) * new_NW);
        NW = new_NW;  // Обновляем размер функции
        n = new_n;  // Обновляем количество переменных
        delete[] temp_func;
    }

    delete[] func_copy;
}







int main()
{
    srand(time(NULL));
    int n = 3;
    BF bf(n,2);
    std::cout << bf << std::endl;
    //bf.print_all_subfunctions(3);
    bf.build_anf();

    for (int k = 1; k <= n; ++k) {
        std::cout << "Subfunctions of size " << k << ":" << std::endl;
        bf.print_all_subfunctions(k);
        std::cout << std::endl;
    }

    return 0;

}

// Задача: 
//вывести ее АНФ И К вывести все подфункции от к переменных. Также вывести все фиксации, т.е если зафиксировали x1, фиксацией ее может быть 1 или 0. Если зафиксировали x1 в 1,
// то анф x1x2 + x1x2x3 примет вид x2 + x2x3 