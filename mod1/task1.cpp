#include <iostream>
#include <cmath>

// Условие: Вернуть значение бита в числе N по его номеру K.

int solve(unsigned long n, unsigned long k)
{
    unsigned long bit;
    unsigned long countBits = 0;
    while (n != 0)
    {
        bit = n & 1;

        if (countBits == k)
        {
            return bit;
        }

        n = n >> 1;
        ++countBits;
    }

    return 0;
}

int main()
{
    unsigned long n, k;
    std::cin >> n >> k;
    std::cout << solve(n, k) << std::endl;
}