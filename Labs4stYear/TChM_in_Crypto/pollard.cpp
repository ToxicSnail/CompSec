#include <iostream>
#include <vector>
#include <cmath>
#include "big_number.h"

BN f(BN x, BN n) 
{
    BN result = x;
    BN one;
    one = 1;
    //result.square();
    //result = (result*result)
    result = (result*result + one) % n;
    return result;
}

BN P0_Pollard_method(BN n)
{
    BN two, zero;
    two = 2;

    if (n % 2 == zero) 
        return two;

    BN a, b, d, one;
    a = 2;
    b = 2;
    one = 1;
    d = 1;
    while (d == one) 
    {
        a = f(a, n);
        b = f(f(b, n), n);

        if (a == b) 
        {
            std::cout << "No result\n";
            return zero;
        }
        if (a > b)
        {
            d = n.gcd(a - b);
        }
        else
        {
            d = n.gcd(b - a);
        }
    } 
    return d;
}

BN P1_Pollard_method(BN n) 
{

    int B = 10;
    BN a, two, one, zero;
    one = 1;
    two = 2;
    std::cout << "test";
    a = random_bound(two, n - two);
    a.cout_base10();
    BN d = a.gcd(n);
    if (d > one)
        return d;
    int q = 2;
    while (q < B) 
    {
        BN check_prime;
        check_prime = q;
        if (check_prime.isPrime(3)) 
        {
            int e = n.log(q);
            a = fastPow(a, std::pow(q, e), n);
        }
        q++;
        std::cout << q;
    }
    if (a == one) 
    {
        std::cout << "Denied soft\n";
        return zero;
    }
    else 
    {
        d = n.gcd(a - one);
        if (d == one) 
        {
            std::cout << "Denied hard\n";
            return zero;
        }
        else
            return d;
    }
}

int main() 
{
    BN num;

    while (true)
    {
        num.cin_base10();

        BN p1, p2;

        // p1 = P0_Pollard_method(num);
        // cout << "p0   Pollard: ";
        // p1.cout_base10();
        // std::cout << endl;

        p2 = P1_Pollard_method(num);
        cout << "p-1 Pollard: ";
        p2.cout_base10();
        std::cout << endl << endl;
    }
}
