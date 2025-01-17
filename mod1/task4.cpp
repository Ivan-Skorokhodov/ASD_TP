#include <cassert>
#include <iostream>

/*
Решение всех задач данного раздела предполагает использование кучи, реализованной в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.
Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
Требования: время работы O(N * logK). Ограничение на размер кучи O(K)..
Формат входных данных: Сначала вводится количество массивов K. Затем по очереди размер каждого массива и элементы массива. Каждый массив упорядочен по возрастанию.
Формат выходных данных: Итоговый отсортированный массив.
*/
const int DefaultInitialSize = 2;

template <typename T>
class Array
{
public:
    Array() : buffer(0), buffer_size(0), real_size(0) {}
    Array(const Array<T> &other)
    {
        buffer_size = other.buffer_size;
        real_size = other.real_size;
        buffer = new T[buffer_size];
        for (int i = 0; i < real_size; i++)
        {
            buffer[i] = other.buffer[i];
        }
    }
    Array(int amountElements)
    {
        this->buffer = new T[amountElements];
        this->buffer_size = amountElements;
        this->real_size = amountElements;
    };
    ~Array()
    {
        if (buffer)
        {
            delete[] buffer;
        }
    }

    int size() const { return real_size; }

    T getAt(int index) const;
    T operator[](int index) const { return getAt(index); }
    T &operator[](int index);

    void pushBack(T element);

    T popBack();

    Array<T> &operator=(const Array<T> &other)
    {
        if (this != &other)
        {
            delete[] buffer;
            buffer_size = other.buffer_size;
            real_size = other.real_size;
            buffer = new T[buffer_size];
            for (int i = 0; i < real_size; i++)
            {
                buffer[i] = other.buffer[i];
            }
        }
        return *this;
    }

private:
    T *buffer;
    int buffer_size;
    int real_size;
    void grow();
};

template <typename T>
T &Array<T>::operator[](int index)
{
    assert(index >= 0 && index < real_size && buffer != 0);
    return buffer[index];
}

template <typename T>
T Array<T>::getAt(int index) const
{
    assert(index >= 0 && index < real_size && buffer != 0);
    return buffer[index];
}

template <typename T>
void Array<T>::grow()
{
    int new_buffer_size = std::max(buffer_size * 2, DefaultInitialSize);
    T *new_buffer = new T[new_buffer_size];
    for (int i = 0; i < real_size; ++i)
        new_buffer[i] = buffer[i];
    delete[] buffer;
    buffer = new_buffer;
    buffer_size = new_buffer_size;
}

template <typename T>
void Array<T>::pushBack(T element)
{
    if (real_size == buffer_size)
        grow();
    assert(real_size < buffer_size && buffer != 0);
    buffer[real_size++] = element;
}

template <typename T>
T Array<T>::popBack()
{
    assert(real_size != 0);
    T element = getAt(real_size - 1);
    --real_size;
    return element;
}

template <typename T>
class IsLessDefault
{
public:
    bool operator()(const T &l, const T &r) { return l < r; }
};

template <typename T, typename IsLess = IsLessDefault<T>>
class Heap
{
public:
    explicit Heap(const Array<T> &_arr, IsLess _cmp = IsLess()) : arr(_arr), cmp(_cmp)
    {
        this->buildHeap();
    }

    ~Heap() {};

    void Insert(T element);
    T ExtractMin();
    int Size();

    Heap(const Heap &) = delete;
    Heap(Heap &&) = delete;
    Heap &operator=(const Heap &) = delete;
    Heap &operator=(Heap &&) = delete;

private:
    Array<T> arr;
    IsLess cmp;
    void buildHeap();
    void siftDown(int i);
    void siftUp(int i);
};

template <typename T, typename IsLess>
void Heap<T, IsLess>::siftDown(int i)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    int smallest = i;
    if (left < arr.size() && cmp(arr[left], arr[i]))
        smallest = left;
    if (right < arr.size() && cmp(arr[right], arr[smallest]))
        smallest = right;

    if (smallest != i)
    {
        T t;
        t = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = t;

        siftDown(smallest);
    }
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::buildHeap()
{
    for (int i = arr.size() / 2 - 1; i >= 0; --i)
    {
        siftDown(i);
    }
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::siftUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (!cmp(arr[index], arr[parent]))
            return;

        T t;
        t = arr[index];
        arr[index] = arr[parent];
        arr[parent] = t;

        index = parent;
    }
}

template <typename T, typename IsLess>
void Heap<T, IsLess>::Insert(T element)
{
    arr.pushBack(element);
    siftUp(arr.size() - 1);
}

template <typename T, typename IsLess>
T Heap<T, IsLess>::ExtractMin()
{
    assert(arr.size() != 0);

    T result = arr[0];

    arr[0] = arr.getAt(arr.size() - 1);
    arr.popBack();

    if (arr.size() != 0)
    {
        siftDown(0);
    }
    return result;
}

template <typename T, typename IsLess>
int Heap<T, IsLess>::Size()
{
    return arr.size();
}

class Descriptor
{
public:
    Array<int> *arr;
    int pos;

    Descriptor() : arr(nullptr), pos(0) {}
    Descriptor(Array<int> *_arr, int _pos) : arr(_arr), pos(_pos) {}
};

class IsLessCustom
{
public:
    IsLessCustom(int _i) : i(_i) {}

    bool operator()(const Descriptor &l, const Descriptor &r)
    {
        return l.arr->getAt(l.pos) < r.arr->getAt(r.pos);
    }

private:
    int i;
};

void solve(int k)
{
    int n, elem;
    Array<Descriptor> matrix(k);

    for (int i = 0; i < k; ++i)
    {
        std::cin >> n;

        Array<int> *arr = new Array<int>();

        for (int j = 0; j < n; j++)
        {
            std::cin >> elem;
            arr->pushBack(elem);
        }

        Descriptor desc(arr, 0);
        matrix[i] = desc;
    }

    IsLessCustom cmpCustom(0);
    Heap<Descriptor, IsLessCustom> h(matrix, cmpCustom);

    while (h.Size() > 0)
    {
        Descriptor top = h.ExtractMin();

        std::cout << top.arr->getAt(top.pos) << " ";

        if (top.pos + 1 < top.arr->size())
        {
            top.pos++;
            h.Insert(top);
        }
    }

    for (int i = 0; i < k; ++i)
    {
        delete matrix[i].arr;
    }
}

int main()
{
    int k;
    std::cin >> k;

    solve(k);

    return 0;
}