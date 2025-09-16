#ifndef _POLINOMIAL_H_
#define _POLINOMIAL_H_


#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

class Polynomial
{
private:
    vector<int> coefficients;
    int mod;

public:
    static int modInverse(int a, int mod)
    {
        int m0 = mod, t, q;
        int x0 = 0, x1 = 1;

        if (mod == 1)
            return 0;

        while (a > 1)
        {
            q = a / mod;
            t = mod;

            mod = a % mod;
            a = t;
            t = x0;

            x0 = x1 - q * x0;
            x1 = t;
        }

        if (x1 < 0)
            x1 += m0;

        return x1;
    }

    void normalize()
    {
        while (!coefficients.empty() && coefficients.back() == 0)
        {
            coefficients.pop_back();
        }
        if (coefficients.empty())
            coefficients.push_back(0);
    }

    Polynomial(const vector<int>& coeffs, int mod) : coefficients(coeffs), mod(mod)
    {
        normalize();
    }

    void print() const
    {
        bool first = true;
        for (int i = degree(); i >= 0; --i)
        {
            if (coefficients[i] != 0)
            {
                if (!first)
                    cout << " + ";
                if (i == 0)
                {
                    cout << coefficients[i];
                }
                else if (i == 1)
                {
                    cout << coefficients[i] << "*x";
                }
                else
                {
                    cout << coefficients[i] << "*x^" << i;
                }
                first = false;
            }
        }
        if (first)
            cout << "0";
        cout << endl;
    }

    int degree() const
    {
        return coefficients.size() - 1;
    }

    Polynomial operator+(const Polynomial& other) const
    {
        vector<int> result(max(coefficients.size(), other.coefficients.size()), 0);

        for (int i = 0; i < result.size(); ++i)
        {
            int a = (i < coefficients.size()) ? coefficients[i] : 0;
            int b = (i < other.coefficients.size()) ? other.coefficients[i] : 0;
            result[i] = (a + b) % mod;
        }

        return Polynomial(result, mod);
    }

    Polynomial operator-(const Polynomial& other) const
    {
        vector<int> result(max(coefficients.size(), other.coefficients.size()), 0);

        for (int i = 0; i < result.size(); ++i)
        {
            int a = (i < coefficients.size()) ? coefficients[i] : 0;
            int b = (i < other.coefficients.size()) ? other.coefficients[i] : 0;
            result[i] = (a - b + mod) % mod;
        }

        return Polynomial(result, mod);
    }

    Polynomial operator*(const Polynomial& other) const
    {
        std::vector<int> result(coefficients.size() + other.coefficients.size() - 1, 0);

        for (int i = 0; i < coefficients.size(); ++i)
        {
            for (int j = 0; j < other.coefficients.size(); ++j)
                result[i + j] = (result[i + j] + (coefficients[i] * other.coefficients[j]) % mod) % mod;
        }

        return Polynomial(result, mod);
    }

    Polynomial operator%(const Polynomial& divisor) const
    {
        Polynomial dividend = *this;
        Polynomial result({ 0 }, mod);

        if (divisor.degree() == 0)
        {
            if (divisor.coefficients[0] == 0)
            {
                throw std::invalid_argument("Division by zero polynomial is not allowed.");
            }

            return Polynomial({ 0 }, mod);
        }

        while (dividend.degree() >= divisor.degree())
        {
            int degreeDiff = dividend.degree() - divisor.degree();
            vector<int> quotientTerm(degreeDiff + 1, 0);
            quotientTerm[degreeDiff] = modInverse(divisor.coefficients.back(), mod) * dividend.coefficients.back() % mod;

            Polynomial term(quotientTerm, mod);
            result = result + term;
            dividend = dividend - (term * divisor);
        }

        return dividend;
    }

    bool operator!=(const Polynomial& x) const
    {
        if (this->coefficients.size() != x.coefficients.size())
        {
            return true;
        }
        for (int i = 0; i < this->coefficients.size(); i++)
        {
            if (this->coefficients[i] != x.coefficients[i])
            {
                return true;
            }
        }
        return false;
    }

    bool operator==(const Polynomial& x) const {
        if (this->coefficients.size() != x.coefficients.size())
        {
            return false;
        }
        for (int i = 0; i < this->coefficients.size(); i++)
        {
            if (this->coefficients[i] != x.coefficients[i])
            {
                return false;
            }
        }
        return true;
    }

    Polynomial pow(int deg)
    {
        Polynomial f = *this;
        Polynomial result = f;
        while (deg != 1)
        {
            result = result * f;
            deg--;
        }
        return result;
    }

    Polynomial gcd(const Polynomial& b) const
    {
        Polynomial f = *this;
        Polynomial g = b;

        if (f.degree() == 0 && f.coefficients[0] == 1) // f = 1
            return f;
        if (g.degree() == 0 && g.coefficients[0] == 1) // g = 1
            return g;

        while (g.degree() >= 0 && !(g.degree() == 0 && g.coefficients[0] == 0)) // Многочлен - старший коэфф - 1
        {
            Polynomial remainder = f % g;
            f = g;
            g = remainder;
        }

        int leadingCoeff = f.coefficients.back();
        int inverseLeadingCoeff = modInverse(leadingCoeff, f.mod);

        for (int& coeff : f.coefficients)
        {
            coeff = (coeff * inverseLeadingCoeff) % f.mod;
            if (coeff < 0)
                coeff += f.mod;
        }

        return f;
    }

    bool irreducibility1()  // тест
    {
        Polynomial f = *this;
        Polynomial one({ 1 }, f.mod);
        Polynomial x({ 0, 1 }, f.mod);
        int deg = f.degree();

        Polynomial u = x;

        for (int i = 0; i < int(deg / 2); i++)
        {
            u = (u.pow(f.mod)) % f;


            Polynomial tmp = u - x;
            Polynomial d = f.gcd(tmp);

            if (d != one)
                return true;
        }
        return false;
    }

    vector<int> primeFactorization(int n)
    {
        vector<int> primeNumber;
        while (n % 2 == 0)
        {
            primeNumber.push_back(2);
            n = n / 2;
        }

        for (int i = 3; i < sqrt(n); i++)
        {
            while (n % i == 0)
            {
                primeNumber.push_back(i);
                n = n / i;
            }
        }

        if (n > 2)
        {
            primeNumber.push_back(n);
        }
        return primeNumber;
    }

    bool irreducibility2()
    {
        Polynomial f = *this;
        Polynomial x({ 0, 1 }, f.mod);
        Polynomial one({ 1 }, f.mod);
        int deg = std::pow(f.mod, f.degree());
        Polynomial u = x.pow(deg) % f;

        if (u != x)
            return true;

        vector<int> factorization = primeFactorization(f.degree());

        for (int k = 0; k < factorization.size(); k++)
        {
            int deg_x = std::pow(f.mod, (f.degree() / factorization[k]));   //p^(n/p(i))
            u = x.pow(deg_x) % f;

            Polynomial d = f.gcd(u - x);

            if (d != one)
                return true;
        }
        return false;
    }

    bool primitiveness()
    {

        Polynomial f = *this;
        if (f.irreducibility2() == true)
        {
            return false;
        }
        Polynomial x({ 0, 1 }, f.mod);
        Polynomial one({ 1 }, f.mod);

        int deg = f.degree();
        int p = std::pow(f.mod, deg) - 1;

        vector<int> factorization = primeFactorization(p);

        int q = factorization[0];

        for (int k = 0; k < factorization.size(); k++)
        {
            int deg_x = p / q;
            Polynomial r = x.pow(deg_x) % f;
            if (r == one)
            {
                return false;
            }
        }
        return true;
    }
};

#endif
