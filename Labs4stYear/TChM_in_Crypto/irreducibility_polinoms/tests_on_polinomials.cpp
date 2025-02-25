#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "polinomial.h"

using namespace std;

void run_tests_2(int choice)
{
    vector<vector<int>> irreducible_polynomials =
    {
        {1, 0, 0, 0, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 1, 1 ,1},
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1}
    };

    vector<vector<int>> reducible_polynomials =
    {
        {1, 0, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 1, 1},
        {1, 0, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1}
    };

    int p = 2;

    cout << "Тестируем неприводимые многочлены z = 2:\n";
    for (const auto& poly : irreducible_polynomials)
    {
        Polynomial test_poly(poly, p);
        if (choice == 1)
        {
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
        else if (choice == 2)
        {
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "\nТестируем приводимые многочлены z = 2:\n";
    for (const auto& poly : reducible_polynomials)
    {
        if (choice == 1)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
        else if (choice == 2)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}


void run_tests_3(int choice)
{
    vector<vector<int>> irreducible_polynomials =
    {
        {1, 0, 0, 0, 2, 1},
        {1, 0, 0, 0, 2, 2},
        {1, 0, 0, 1, 1, 2},
        {1, 0, 0, 2, 1, 1},
        {1, 0, 2, 1, 2, 2},
        {1, 0, 0, 0, 0, 1, 2},
        {1, 0, 0, 0, 0, 2, 2},
        {1, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 2 ,1},
        {1, 0, 0, 0, 2, 0, 1}
    };

    vector<vector<int>> reducible_polynomials =
    {
        {1, 0, 0, 0, 1, 2},
        {1, 0, 0, 2, 1, 2},
        {1, 0, 0, 2, 2, 1},
        {1, 0, 1, 2, 0, 2},
        {1, 0, 2, 1, 0, 2},
        {1, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 1, 2, 2},
        {1, 0, 0, 0, 1, 1, 2},
        {1, 0, 0, 0, 2, 1, 1},
        {1, 0, 0, 0, 2, 1 ,2}
    };

    int p = 3;

    cout << "Тестируем неприводимые многочлены z = 3:\n";
    for (const auto& poly : irreducible_polynomials)
    {
        Polynomial test_poly(poly, p);
        if (choice == 1)
        {
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
        else if (choice == 2)
        {
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "\nТестируем приводимые многочлены z = 3:\n";
    for (const auto& poly : reducible_polynomials)
    {
        if (choice == 1)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
        else if (choice == 2)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}

void run_tests_5(int choice)
{
    vector<vector<int>> irreducible_polynomials =
    {
        {1, 0, 0, 0, 4, 1},
        {1, 0, 0, 0, 4, 2},
        {1, 0, 0, 0, 4, 3},
        {1, 0, 0, 0, 4, 4},
        {1, 0, 0, 1, 2, 4},
        {1, 0, 0, 0, 2},
        {1, 0, 0, 1, 4},
        {1, 0, 1, 0, 2},
        {1, 0, 1, 2, 2},
        {1, 0, 1, 3, 2}
    };

    vector<vector<int>> reducible_polynomials =
    {
        {1, 0, 0, 0, 3, 1},
        {1, 0, 0, 0, 3, 4},
        {1, 0, 0, 0, 2, 1},
        {1, 0, 0, 1, 1, 2},
        {1, 0, 0, 1, 2, 3},
        {1, 0, 0, 0, 4},
        {1, 0, 0 ,1 ,2},
        {1, 0, 0, 1, 3},
        {1, 0, 1, 1, 2},
        {1, 0, 1, 2, 4}
    };

    int p = 5;

    cout << "Тестируем неприводимые многочлены z = 5:\n";
    for (const auto& poly : irreducible_polynomials)
    {
        Polynomial test_poly(poly, p);
        if (choice == 1)
        {
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
        else if (choice == 2)
        {
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим (ошибка)\n";
            }
            else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "\nТестируем приводимые многочлены z = 5:\n";
    for (const auto& poly : reducible_polynomials)
    {
        if (choice == 1)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility1())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
        else if (choice == 2)
        {
            Polynomial test_poly(poly, p);
            if (test_poly.irreducibility2())
            {
                cout << "Полином приводим\n";
            }
            else
            {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}

void tests_primitiveness()
{
    Polynomial test({ 2, 1, 1, 0, 0, 1 }, 5);

    test.print();
    if (test.primitiveness() == true) 
    {
        cout << "true" << endl;
    }
    else {
        cout << "false" << endl;
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");

    short int choice = 0;
    cout << "Введите число\n(1 - тест 1 на неприводимость полиномов\n2 - тест 2 на неприводимость полиномов): ";
    cin >> choice;
    if (choice > 0 && choice <= 2)
    {
        run_tests_2(choice);
        cout << endl;
        run_tests_3(choice);
        cout << endl;
        run_tests_5(choice);
        cout << endl;
        tests_primitiveness();
        return 0;

    }
}
