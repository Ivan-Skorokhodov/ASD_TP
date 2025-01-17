#include <iostream>

int lower_bound(const int *arr, int count, int element)
{
    int first = 0;
    int last = count;
    while (first < last)
    {
        int mid = (first + last) / 2;
        if (arr[mid] < element)
        {
            first = mid + 1;
        }
        else
        {
            last = mid;
        }
    }
    return first;
}

int main()
{
    int n, element;
    std::cin >> n;

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    std::cin >> element;
    std::cout << lower_bound(arr, n, element) << std::endl;

    return 0;
}