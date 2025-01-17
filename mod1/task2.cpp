#include <iostream>
#include <cmath>

/*
Дан массив целых чисел А[0..n-1].
Известно, что на интервале [0, m] значения массива строго возрастают, а на интервале [m, n-1] строго убывают.
Найти m за O(log m).
*/

int binarySearch(const long *arr, int posLeft, int posRight)
{
    int left = posLeft;
    int right = posRight;
    while (left < right)
    {
        int mid = (left + right) / 2;

        if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1])
        {
            return mid;
        }

        if (arr[mid] > arr[mid - 1])
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }

    if (arr[posLeft] > arr[posRight])
    {
        return posLeft;
    }
    return posRight;
}

void findPositions(const long *arr, int *posLeft, int *posRight, int size)
{
    if (size == 2)
    {
        *posRight = 1;
        *posLeft = 0;
        return;
    }

    int right = 1;
    while (arr[right] > arr[right - 1] && right < size)
    {
        right = right << 1;
    }

    if (right >= size)
    {
        *posRight = size - 1;
        *posLeft = right >> 1;
    }
    else
    {
        *posRight = right;
        *posLeft = right >> 1;
    }
}

int main()
{
    int n;
    std::cin >> n;

    long arr[n];
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i];
    }

    int posLeft, posRight;
    findPositions(arr, &posLeft, &posRight, n);

    std::cout << binarySearch(arr, posLeft, posRight) << std::endl;
}