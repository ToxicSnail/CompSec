#include <iostream>
#include <chrono>

using namespace std;

class Array {
    int* a, count;
public:
    Array(int len = 1, int t = 1, int d = 10) // d – диапазон псевдослучайных чисел для неупорядоченного массива (при t = 1)
    {
        a = new int[len];
        count = len;                // len – число элементов в массиве
        if (t == 1)
        {                           // t = 1 – неупорядоченный массив
            for (int i = 0; i < count; i++)
            {
                a[i] = rand() % d;
            }
        }
        else if (t == 2) // t = 2 – массив, упорядоченный по неубыванию
        {
            a[0] = rand() % d;
            for (int i = 1; i < count; i++)
            {
                a[i] = a[i - 1] + rand() % d;
            }
        }
        else if (t == 3) // t = 3 – массив, упорядоченный по невозрастанию
        {
            a[count - 1] = rand() % d;
            for (int i = count - 2; i >= 0; i--)
            {
                a[i] = a[i + 1] + rand() % d;
            }
        }
        else exit(1);
    };

    Array(int* b, int len) // конструктор 2: по массиву
    {
        a = new int[len];
        count = len;
        for (int i = 0; i < count; i++)
        {
            a[i] = b[i];
        }
    };

    Array(Array& b) //конструктор копирования
    {
        a = new int[b.count];
        count = b.count;
        for (int i = 0; i < count; i++)
        {
            a[i] = b.a[i];
        }
    };

    ~Array() //деструктор
    {
        if (a) delete[]a;
        a = NULL;
    }

    Array& operator=(const Array&);     //оператор присваивания
    int& operator[](int);               //оператор для получения i-ого элемента

    bool Test(int* array, int size);
    bool operator==(Array);             // равенство элементов массивов (но не порядка)

    void Shell_sort();

    void Heapsort();

    void heapify(int, int);

    void Hoar_sort();

    //void Bit_sort(int, int, int, bool);

    int major_bit();                    // для Bit_sort

    friend istream& operator>>(istream&, Array&);

    friend ostream& operator<<(ostream&, Array&);
};

Array& Array::operator=(const Array& newArr) //оператор присваивания
{
    if (this == &newArr)
        return *this;
    delete[] a;
    count = newArr.count;
    a = new int[count];
    for (int i = 0; i < count; i++)
    {
        a[i] = newArr.a[i];
    }
    return *this;
};

int& Array::operator[](int b) //оператор для получения i-ого элемента
{
    if (b < 0 || b >= count) return a[0];
    return a[b];
}

bool check_sorted_array(Array array, int size) //проверка на отсортированность массива
{
	if (size == 1)
		return true;
	for (int i = 0; i < size - 1; i++)
	{
		if ((array[i] > array[i + 1]))
		{
			return false;
		}
	}
	return true;
}


bool Array::operator==(Array b) // равенство элементов массивов (но не порядка)
{
    if (count != b.count) return false;
    int len = count;
    for (int i = 0; i < count; i++)
    {
        bool ch = false;
        for (int j = 0; j < len; j++)
        {
            if (a[i] == b.a[j])
            {
                ch = true;
                b[j] = b[len - 1];
                len--;
                break;
            }
        }
        if (ch == false) return false;
    }
    return true;
}

istream& operator>>(istream& in, Array& obj) //оператор ввода
{
    int len;
    cout << "Length: ";
    in >> len;
    cout << "\n Input your array: ";
    int* arr;
    arr = new int[len];
    for (int i = 0; i < len; i++) {
        in >> arr[i];
    }
    obj = Array(arr, len);
    delete[] arr;
    return in;
}

ostream& operator<<(ostream& out, Array& a) //оператор вывода
{
    /*
    if (check_sorted_array(a, a.count))
    {
        out << "Array has been sorted. Your sorted array: ";
        for (int i = 0; i < a.count; i++)
        {
            out << a[i] << " ";
        }
        out << "\n";
    }
    else
    {
        out << "Array wasn't been sorted";
    }
    return out;
    */
    out << "    ";
    for (int i = 0; i < a.count; i++)
    {
        out << a[i] << " ";
    }
    return out;
}

void Array::Shell_sort() //сортировка Шела
{
    for (int gap = count / 2; gap > 0; gap /= 2) //разбиваем на шаги
    {
        for (int b = 0; b < gap; b++)   //0 до шаг ( 1 элемент нашего подмассива)
        {
            for (int i = b + gap; i < count; i += gap) //следующий элемент
            {
                int x = a[i];
                int k;
                for (k = i - gap; k >= 0 && a[k] > x; k -= gap) //смотрим на предыдущие элементы и сравниваем с нашим
                {
                    a[k + gap] = a[k];
                }
                a[k + gap] = x;
            }
        }
    }
}

int change(int* arr, int l, int r) //функция сортировки для удобства
{
    if (l >= r) //выход из рекурсии, когда массив состоит из 1 элемента
    {
        return 0;
    }
    int i = l;  //индекс i приравнивается к минимальному индексу разделяемого массива.
    int j = r;  //индекс j приравнивается к максимальному индексу разделяемого массива.
    int x = arr[(i + j) / 2];   //вычисляется индекс опорного элемента x
    int tmp;
    while (i <= j)
    {
        while (arr[i] < x)  //индекс i последовательно увеличивается до x до тех пор, пока i-й элемент не превысит опорный.
        {
            i++;
        }
        while (arr[j] > x)  //индекс j последовательно уменьшается до x до тех пор, пока j-й элемент не окажется меньше либо равен опорному.
        {
            j--;
        }
        if (i <= j)                 //найденную пару элементов нужно обменять местами и продолжить операцию разделения 
        {                               //с тех значений i и j, которые были достигнуты. 
            //swap(arr[i], arr[j]);   //
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;                    //
            j--;                    //
        }
    }

    change(arr, l, j);  //рекурсивно упорядочиваем подмассивы, лежащие слева и справа от опорного элемента
    change(arr, i, r);  //(рекурсивно выполняем алгоритм для правого и левого образовавшегося участка. Для левого участка - левая граница сохраняется, правая равна j. Для правого - левая равна i, правая - правая граница сохраняется)
    return 0;
}

void Array::Hoar_sort() //сортировка Хоара - быстрая сортировка, а та в свою очередь выбирается эталонное значение
{
    change(a, 0, count - 1);    //вызываем функцию обмена(и есть алгоритм Хоара) для массива, передовая его левый и правый индекс
}

void Array::heapify(int n, int i) //функция просеивания
{
    int j = 2 * i + 1;  //левый = 2*i + 1
    int x = a[i];
    bool f = true;
    while (j < n && f)
    {
        if (j + 1 < n && a[j + 1] > a[j]) j++;
        if (a[j] > x)
        {
            a[i] = a[j];
            i = j;
            j = 2 * i + 1;
        }
        else f = false;
    }
    a[i] = x;
}

void Array::Heapsort() //пирамидальная сортировка
{
    for (int i = count / 2 - 1; i >= 0; i--)    //строим основание пирамиды для массива a.    /*построение кучи (перегруппируем массив)*/
        this->heapify(count, i);                //элементы a[n/2], a[n/2]+1,…,an-1 образуют основание пирамиды, так как у них нет потомков.
    for (int i = count - 1; i > 0; i--) //один за другим извлекаем элементы из кучи
    {
        swap(a[0], a[i]);   //перемещаем текущий корень в конец
        this->heapify(i, 0);    //вызываем процедуру heapify на уменьшенной куче
    }
}

/*
void Array::Bit_sort(int l = 0, int r = 0, int k = 0, bool flag = true) //побитовая сортировка
{
    if (flag)
    {
        flag = false;
        l = 0;
        r = count - 1;
        k = this->major_bit();
    }
    if (!(l >= r || k < 0))
    {
        int i = l, j = r;
        int mask = 1 << k;
        while (i <= j)
        {
            while (i <= j && !(a[i] & mask)) i++;
            while (i <= j && a[j] & mask) j--;
            if (i < j) swap(a[i++], a[j--]);
        }
        this->Bit_sort(l, j, k - 1, flag);
        this->Bit_sort(i, r, k - 1, flag);
    }
}
int Array::major_bit() //поиск главного бита
{
    int max = a[0];
    for (int i = 0; i < count; i++)
    {
        if (a[i] > max) max = a[i];
    }
    int k = 0;
    while (max)
    {
        max >>= 1;
        k++;
    }
    return k - 1;
}
*/
void test()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    Array t(100000, 1, 100000);
    //Array t1(100000, 1);
    Array t2 = t;
    Array t3 = t;


    double seconds = 0;

    auto time1 = high_resolution_clock::now();
    t.Shell_sort();
    auto time2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = time2 - time1;
    cout << "\nTime sort by Shell_sort " << ms_double.count() << " ms" << endl;
    if (check_sorted_array(t, 100000)) printf("    Array has been sorted\n");
    else printf("    Array hasn't been sorted\n");

    /*
    time1 = high_resolution_clock::now();
    t1.Bit_sort();
    time2 = high_resolution_clock::now();
    ms_double = time2 - time1;
    cout << "Time sort by Bit_sort " << ms_double.count() << " ms" << endl;
    */
    time1 = high_resolution_clock::now();
    t2.Heapsort();
    time2 = high_resolution_clock::now();
    ms_double = time2 - time1;
    cout << "Time sort by Heapsort " << ms_double.count() << " ms" << endl;
    if (check_sorted_array(t2, 100000)) printf("    Array has been sorted\n");
    else printf("    Array hasn't been sorted\n");

    time1 = high_resolution_clock::now();
    t3.Hoar_sort();
    time2 = high_resolution_clock::now();
    ms_double = time2 - time1;
    std::cout << "Time sort by Hoar_sort: " << ms_double.count() << " ms" << endl;
    if (check_sorted_array(t3, 100000)) printf("    Array has been sorted\n");
    else printf("    Array hasn't been sorted\n");

    if ((t == t2) && (t == t3))
        printf("\nSame");
    else printf("\nDifferent");
}

int main()
{
    Array t;
    cin >> t;
    t.Hoar_sort();
    cout << t;
    test();

    return 0;
}
