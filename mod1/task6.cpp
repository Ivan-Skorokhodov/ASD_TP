#include <iostream>

/*
Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

Требования: к дополнительной памяти: O(1).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
*/

template <typename T>
struct DefaultComparator
{
    bool operator()(const T &l, const T &r) const
    {
        return l < r;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T *arr, int l, int r, Comparator comp = Comparator())
{
    // T pivot = arr[r - 1];
    int pivot_ind = l + (rand() % (r - l));
    std::swap(arr[pivot_ind], arr[r - 1]);
    T pivot = arr[r - 1];

    int i = l - 1;

    for (int j = l; j < r - 1; j++)
    {
        if (comp(arr[j], pivot))
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[r - 1]);
    return i + 1;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_element(T *arr, int size, int k, Comparator comp = Comparator())
{
    if (k < 0 || k >= size)
        return;

    int l = 0;
    int r = size;

    while (l < r)
    {
        int pivot_pos = partition(arr, l, r, comp);
        if (pivot_pos == k)
        {
            return;
        }
        if (k < pivot_pos)
        {
            r = pivot_pos;
        }
        else
        {
            l = pivot_pos + 1;
        }
    }
}

void solve(int n, int *arr)
{
    int k = static_cast<int>(n * 0.1);
    kth_element(arr, n, k);
    std::cout << arr[k] << std::endl;

    k = static_cast<int>(n * 0.5);
    kth_element(arr, n, k);
    std::cout << arr[k] << std::endl;

    k = static_cast<int>(n * 0.9);
    kth_element(arr, n, k);
    std::cout << arr[k] << std::endl;
}

int main()
{
    int n;
    std::cin >> n;
    int arr[n];
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i];
    }

    solve(n, arr);

    return 0;
}