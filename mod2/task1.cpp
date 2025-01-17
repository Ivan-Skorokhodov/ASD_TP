#include <vector>
#include <iostream>
#include <cassert>
#include <string>

/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Для разрешения коллизий используйте двойное хеширование.
*/

template <typename T, typename H1, typename H2>
class HashTable
{
public:
    HashTable(int initialSize, H1 keyHasher, H2 probHasher);
    ~HashTable();

    bool Has(const T &key) const;
    bool AddKey(const T &key);
    bool DeleteKey(const T &key);

    HashTable(const HashTable &) = delete;
    HashTable(HashTable &&) = delete;
    HashTable &operator=(const HashTable &) = delete;
    HashTable &operator=(HashTable &&) = delete;

private:
    enum CellState
    {
        Empty,
        HasKey,
        Deleted
    };
    struct HashTableNode
    {
        T Key{};
        unsigned int Hash = 0;
        CellState State = Empty;
        HashTableNode() = default;
    };
    std::vector<HashTableNode> table;
    H1 keyHasher;
    H2 probHasher;
    int keysCount;
    int deletedCount;

    void growTable(int coefficient = 2);
};

template <typename T, typename H1, typename H2>
HashTable<T, H1, H2>::HashTable(int initialSize, H1 _keyHasher, H2 _probHasher) : keyHasher(_keyHasher),
                                                                                  probHasher(_probHasher),
                                                                                  table(initialSize),
                                                                                  keysCount(0),
                                                                                  deletedCount(0)
{
}

template <typename T, typename H1, typename H2>
HashTable<T, H1, H2>::~HashTable()
{
}

template <typename T, typename H1, typename H2>
bool HashTable<T, H1, H2>::Has(const T &key) const
{
    unsigned int hash = keyHasher(key);
    unsigned int listIndex = hash % table.size();

    while ((table[listIndex].State == Deleted || table[listIndex].Key != key) && table[listIndex].State != Empty)
    {
        listIndex += probHasher(key);
        listIndex %= table.size();
    }

    if (table[listIndex].State == Empty)
    {
        return false;
    }

    return true;
}

template <typename T, typename H1, typename H2>
bool HashTable<T, H1, H2>::AddKey(const T &key)
{
    if (3 * keysCount > table.size())
    {
        growTable();
    }

    if (3 * deletedCount > table.size())
    {
        growTable(1);
        deletedCount = 0;
    }

    unsigned int hash = keyHasher(key);
    unsigned int listIndex = hash % table.size();
    unsigned int firstDeleted = -1;

    while ((table[listIndex].State == Deleted || table[listIndex].Key != key) && table[listIndex].State != Empty)
    {
        if (firstDeleted == -1 && table[listIndex].State == Deleted)
        {
            firstDeleted = listIndex;
        }

        listIndex += probHasher(key);
        listIndex %= table.size();
    }

    if (table[listIndex].Key == key)
    {
        return false;
    }

    HashTableNode newNode = HashTableNode();
    newNode.Key = key;
    newNode.Hash = hash;
    newNode.State = HasKey;

    if (firstDeleted != -1)
    {
        table[firstDeleted] = newNode;
    }
    else
    {
        table[listIndex] = newNode;
    }
    ++keysCount;

    return true;
}

template <typename T, typename H1, typename H2>
bool HashTable<T, H1, H2>::DeleteKey(const T &key)
{
    unsigned int hash = keyHasher(key);
    unsigned int listIndex = hash % table.size();

    while ((table[listIndex].State == Deleted || table[listIndex].Key != key) && table[listIndex].State != Empty)
    {
        listIndex += probHasher(key);
        listIndex %= table.size();
    }

    if (table[listIndex].Key == key)
    {
        table[listIndex].State = Deleted;
        --keysCount;
        ++deletedCount;
        return true;
    }

    return false;
}

template <typename T, typename H1, typename H2>
void HashTable<T, H1, H2>::growTable(int coefficient)
{
    std::vector<HashTableNode> newTable(table.size() * coefficient);
    for (int i = 0; i < table.size(); ++i)
    {
        HashTableNode node = table[i];
        if (node.State == HasKey)
        {
            unsigned int listIndex = node.Hash % newTable.size();
            while (newTable[listIndex].State == HasKey)
            {
                listIndex += probHasher(node.Key);
                listIndex %= newTable.size();
            }
            newTable[listIndex] = node;
        }
    }

    table = std::move(newTable);
}

struct StringKeyHasher
{
    unsigned int operator()(const std::string &key) const
    {
        const unsigned int p = 31;
        const unsigned int m = 131072; // 2^17
        unsigned int hashValue = 0;

        for (char c : key)
        {
            hashValue = (hashValue + static_cast<unsigned int>(c)) % m;
        }

        return hashValue;
    }
};

struct StringProbHasher
{
    unsigned int operator()(const std::string &key) const
    {
        const unsigned int p = 31;
        const unsigned int m = 131072; // 2^17
        unsigned int hashValue = 0;

        for (char c : key)
        {
            hashValue = (hashValue + static_cast<unsigned int>(c)) % m;
        }

        return hashValue * 2 - 1;
    }
};

int main()
{
    StringKeyHasher keyHasher;
    StringProbHasher probHasher;

    HashTable<std::string, StringKeyHasher, StringProbHasher> table(2, keyHasher, probHasher);

    char operation = 0;
    std::string word;
    while (std::cin >> operation >> word)
    {
        switch (operation)
        {
        case '+':
            std::cout << (table.AddKey(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (table.DeleteKey(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '?':
            std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
            break;
        default:
            assert(false);
        }
    }
    return 0;
}