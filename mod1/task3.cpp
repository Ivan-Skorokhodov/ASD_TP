#include <iostream>
#include <cassert>

/*
Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
Требования: Дек должен быть реализован в виде класса.
*/
class Dequeue
{
public:
    Dequeue() : arr(new long[2]), arrSize(2), left(0), right(0), realSize(0) {}
    ~Dequeue() { delete[] arr; }

    Dequeue(const Dequeue &) = delete;
    Dequeue(Dequeue &&) = delete;
    Dequeue &operator=(const Dequeue &) = delete;
    Dequeue &operator=(Dequeue &&) = delete;

    long size() const { return realSize; }

    void pushBack(long element);
    void pushFront(long element);
    long popBack();
    long popFront();

private:
    long *arr;
    long arrSize;
    long left;
    long right;
    long realSize;
    void grow();
};

void Dequeue::grow()
{
    long newArrSize = arrSize * 2;
    long *newArr = new long[newArrSize];

    for (long i = 0; i < realSize; ++i)
    {
        newArr[i] = arr[(left + i) % arrSize];
    }

    delete[] arr;

    left = 0;
    right = realSize;

    arr = newArr;
    arrSize = newArrSize;
}

void Dequeue::pushBack(long element)
{
    if (realSize == arrSize)
        grow();
    assert(realSize < arrSize && arr != 0);

    arr[right] = element;
    ++right;
    if (right == arrSize)
        right = 0;
    ++realSize;
}

void Dequeue::pushFront(long element)
{
    if (realSize == arrSize)
        grow();

    --left;
    if (left < 0)
        left = arrSize - 1;
    arr[left] = element;
    ++realSize;
}

long Dequeue::popBack()
{
    --realSize;
    --right;
    if (right < 0)
        right = arrSize - 1;
    return arr[right];
}

long Dequeue::popFront()
{
    long element = arr[left];
    ++left;
    if (left == arrSize)
        left = 0;
    --realSize;
    return element;
}

void solve(long n)
{
    Dequeue d;
    bool result = true;
    for (long i = 0; i < n; ++i)
    {
        long command = 0;
        long data = 0;
        std::cin >> command >> data;
        switch (command)
        {
        case 1:
            d.pushFront(data);
            break;
        case 2:
            if (d.size() == 0)
            {
                result = result && data == -1;
            }
            else
            {
                result = result && d.popFront() == data;
            }
            break;
        case 3:
            d.pushBack(data);
            break;
        case 4:
            if (d.size() == 0)
            {
                result = result && data == -1;
            }
            else
            {
                result = result && d.popBack() == data;
            }
            break;
        default:
            assert(false);
        }
    }

    std::cout << (result ? "YES" : "NO") << std::endl;
}

int main()
{
    long n = 0;
    std::cin >> n;
    solve(n);
}
