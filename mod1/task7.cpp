#include <iostream>

/*
7_3. Binary MSD для long long.
Дан массив неотрицательных целых 64-разрядных чисел.
Количество чисел не больше 106. Отсортировать массив методом MSD по битам (бинарный QuickSort).

*/

void msd_sort(unsigned long long *arr, int left, int right, int bit)
{
    if (bit < 0 || right - left <= 1)
        return;

    int i = left, j = right - 1;

    while (i <= j)
    {
        while (i <= j && ((arr[i] >> bit) & 1) == 0)
            i++;
        while (i <= j && ((arr[j] >> bit) & 1) == 1)
            j--;

        if (i < j)
        {
            std::swap(arr[i++], arr[j--]);
        }
    }

    msd_sort(arr, left, i, bit - 1);
    msd_sort(arr, i, right, bit - 1);
}

int main()
{
    int n;
    std::cin >> n;

    unsigned long long arr[n];

    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    msd_sort(arr, 0, n, 63);

    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}