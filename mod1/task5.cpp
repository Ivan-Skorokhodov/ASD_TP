#include <cstring>
#include <iostream>

/*
Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции.
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).
5_1. Реклама.
В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей (два целых числа).
Каждому покупателю необходимо показать минимум 2 рекламы.
Рекламу можно транслировать только в целочисленные моменты времени. Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода или прихода,
то считается, что посетитель успел её посмотреть. Требуется определить минимальное число показов рекламы.
*/

class Customer
{
public:
    Customer(int _start, int _end) : start(_start), end(_end), seenAds(0) {}
    int start;
    int end;
    int seenAds;
};

class IsLessCustom
{
public:
    bool operator()(const Customer *l, const Customer *r)
    {
        if (l->end < r->end)
            return true;
        if (l->end == r->end)
            return l->start > r->start;
        return false;
    }
};

template <typename T>
class IsLessDefault
{
public:
    bool operator()(const T &l, const T &r) { return l < r; }
};

template <typename T, typename IsLess = IsLessDefault<T>>
void Merge(T *firstArray, int lenFirstArray, T *secondArray, int lenSecondArray, T *mergeArray, IsLess cmp = IsLess())
{
    int i = 0;
    int j = 0;

    while (i < lenFirstArray && j < lenSecondArray)
    {
        if (cmp(firstArray[i], secondArray[j]))
        {
            mergeArray[i + j] = firstArray[i];
            ++i;
        }
        else
        {
            mergeArray[i + j] = secondArray[j];
            ++j;
        }
    }

    while (i < lenFirstArray)
    {
        mergeArray[i + j] = firstArray[i];
        ++i;
    }

    while (j < lenSecondArray)
    {
        mergeArray[i + j] = secondArray[j];
        ++j;
    }
}

template <typename T, typename IsLess = IsLessDefault<T>>
void MergeSort(T *a, int aLen, IsLess cmp = IsLess())
{
    if (aLen <= 1)
    {
        return;
    }
    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    MergeSort(a, firstLen, cmp);
    MergeSort(a + firstLen, secondLen, cmp);

    T *c = new T[aLen];
    Merge(a, firstLen, a + firstLen, secondLen, c, cmp);
    for (int i = 0; i < aLen; i++)
    {
        a[i] = std::move(c[i]);
    }
    delete[] c;
}

bool isTimeUsed(int *adTimes, int adCount, int time)
{
    for (int i = 0; i < adCount; i++)
    {
        if (adTimes[i] == time)
            return true;
    }
    return false;
}

int countAdsForCustomer(int *adTimes, int adCount, Customer *customer)
{
    int seen = 0;
    for (int i = 0; i < adCount; i++)
    {
        if (adTimes[i] >= customer->start && adTimes[i] <= customer->end)
        {
            seen++;
        }
    }
    return seen;
}

void solve(int k)
{
    const int MAX_TIME = 100001;
    bool usedTimes[MAX_TIME] = {false};

    Customer **customers = new Customer *[k];
    for (int i = 0; i < k; ++i)
    {
        int start, end;
        std::cin >> start >> end;
        customers[i] = new Customer(start, end);
    }

    IsLessCustom cmp;
    MergeSort(customers, k, cmp);

    int totalAds = 0;

    for (int i = 0; i < k; ++i)
    {
        Customer *curr = customers[i];

        for (int time = curr->end; time >= curr->start && curr->seenAds < 2; --time)
        {
            if (!usedTimes[time])
            {
                usedTimes[time] = true;
                curr->seenAds++;
                totalAds++;

                for (int j = i + 1; j < k; ++j)
                {
                    if (customers[j]->start <= time && time <= customers[j]->end)
                    {
                        customers[j]->seenAds++;
                    }
                }
            }
        }
    }

    std::cout << totalAds << std::endl;

    // Освобождаем память
    for (int i = 0; i < k; ++i)
    {
        delete customers[i];
    }
    delete[] customers;
}

int main()
{
    int k;
    std::cin >> k;

    solve(k);

    return 0;
}