#include <iostream>
#include <vector>
#include <cmath>
#include "big_number.h"
#include <map>

int Gelfonds_algorithm(BN g, BN p, int n, BN a)
{
    int h = sqrt(n) + 1;
    BN b = g.pow(h) % p;


    BN giant_step_keys[h + 1];
    int giant_step_values[h + 1];


    for (int i = 0; i <= h; i++)
    {
        giant_step_values[i] = -1;
    }

    for (int u = 1; u <= h; u++)
    {
        BN key = b.pow(u) % p;
        bool found = false;

        for (int i = 1; i <= h; i++)
        {
            if (giant_step_values[i] != -1 && giant_step_keys[i] == key)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            giant_step_keys[u] = key;
            giant_step_values[u] = u;
        }
    }

    BN small_step_values[h + 1];

    for (int v = 1; v <= h; v++)
    {
        small_step_values[v] = (a * g.pow(v)) % p;
        for (int i = 1; i <= h; i++)
        {
            if (giant_step_values[i] != -1 && small_step_values[v] == giant_step_keys[i])
            {
                return (h * giant_step_values[i] - v) % n;
            }
        }
    }

    return 0;
}


bool tests() {
    std::vector<int> g_list = { 22, 22, 70, 70, 52, 52, 1003, 1003, 77, 77, 8900, 8900 };
    std::vector<int> p_list = { 599, 599, 599, 599, 1571, 1571, 1571, 1571, 10069, 10069, 10069, 10069 };
    std::vector<int> a_list = { 418, 183, 21, 269, 647, 190, 912, 25, 625, 7128, 9458, 7330 };
    std::vector<int> expected_results = { 185, 321, 221, 436, 368, 1023, 146, 86, 156, 835, 1024, 2356 };
    std::vector<int> n_list = { 598, 598, 598, 598, 1570, 1570, 1570, 1570, 10068, 10068, 10068, 10068 };
    bool all_tests_passed = true;

    for (size_t i = 0; i < g_list.size(); ++i) {
        BN g; BN p; BN a;
        g = g_list[i];
        p = p_list[i];
        a = a_list[i];
        int n = n_list[i];

        int result = Gelfonds_algorithm(g, p, n, a);
        if (result != expected_results[i]) {
            std::cout << "Test " << i + 1 << " failed. Expected " << expected_results[i] << ", but got " << result << "\n";
            all_tests_passed = false;
        }
        else 
            std::cout << "Test " << i + 1 << " passed. Expected " << expected_results[i] << ", and got " << result << "\n";

    }

    if (all_tests_passed) {
        std::cout << "All tests passed successfully.\n\n";
    }

    return all_tests_passed;
}

int main() {
    if (tests()) {
        std::cout << "All tests completed successfully!\n";
    }
    else {
        std::cout << "Some tests failed.\n";
    }

    /*
    BN g, p, a;
    int n, temp;
    std::cout << "Enter g:\n";
    g.cin_base10();
    std::cout << "Enter a:\n";
    a.cin_base10();
    std::cout << "Enter p: ";
    std::cin >> temp;
    n = temp - 1;
    p = temp;
    std::cout << Gelfonds_algorithm(g, p, n, a) << "\n";
    */

    return 0;
}
