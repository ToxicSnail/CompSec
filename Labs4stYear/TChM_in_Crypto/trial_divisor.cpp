#include <iostream>
#include <vector>
#include "big_number.h"
#include "miller-rabin.h"
#include <cmath>

using namespace std;

vector<BN> trial_divisor_method(const BN& n)
{
    BN check_num = n;

    BN sqrt_n = check_num.sqrt();
    
    vector<BN> d(3);
    BN temp;
    temp = 3;
    d[0] = temp;
    temp = 5;
    d[1] = temp;
    temp = 7;
    d[2] = temp;

    vector<BN> p;
    int t = -1;
    int k = 0;

    BN one, zero, six, two;
    one = 1; zero = 0; six = 6; two = 2;
    while (check_num != one)
    {
        if (check_num % 2 != zero)
        {
            BN r = check_num % d[k];
            BN q = check_num / d[k];
            if (r == zero)
            {
                t++;
                p.push_back(d[k]);
                check_num = q;
            }
            else 
            {
                if (q > d[k] && d[k] <= sqrt_n)
                {
                    k++;
                    if (k > 2) 
                    {
                        temp = d[k - 2] + six;
                        d.push_back(temp);
                    }
                }
                else
                {
                    t++;
                    p.push_back(check_num);
                    return p;
                }
            }
        }
        else
        {
            t++;
            p.push_back(two);
            check_num = check_num / 2;
        }
    }
    return p;
}

int main()
{
    BN num;
    
    BN one;
    one = 1;
    num.cin_base10();

    if (num < one) std::cout << "Число должно быть больше 1";

    int test = 5; 

    if (test_miller_rabin(num, test)) 
    {
        std::cout << "Число простое. Проверка была совершена с помощью алгоритма Миллера-Рабина\n";
        //error_phi(num, test);

        return 0; 
    }
    else
    {
        std::cout << "Число составное:" << endl;

        vector<BN> p = trial_divisor_method(num);

        num.cout_base10();
        cout << " = ";

        for (size_t i = 0; i < p.size(); i++)
        {
            if (i != 0)
            {
                cout << " * ";
            }
            p[i].cout_base10();
        }
        cout << endl;
    }

    return 1;
}
