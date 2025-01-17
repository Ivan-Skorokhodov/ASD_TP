#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>

struct Node
{
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node &other) const
    {
        return product_name == other.product_name &&
               color == other.color &&
               size == other.size;
    }
};

struct NodeHash
{
    std::size_t operator()(const Node &node) const
    {
        const unsigned int m = 131072; // 2^17
        unsigned int hashValue = 0;

        for (char c : node.product_name)
        {
            hashValue = (hashValue + static_cast<unsigned int>(c)) % m;
        }
        return (hashValue + (node.color << 1) + (node.size << 2)) % m;
    }
};

int main()
{
    std::unordered_set<Node, NodeHash> node_set;
    std::string operation;

    while (std::cin >> operation)
    {
        Node node;
        std::cin >> node.product_name;
        std::cin >> node.color;
        std::cin >> node.size;

        if (operation == "+")
        {
            auto result = node_set.insert(node);
            if (result.second)
            {
                std::cout << "OK\n";
            }
            else
            {
                std::cout << "FAIL\n";
            }
        }
        else if (operation == "-")
        {
            auto result = node_set.erase(node);
            if (result > 0)
            {
                std::cout << "OK\n";
            }
            else
            {
                std::cout << "FAIL\n";
            }
        }
        else if (operation == "?")
        {
            if (node_set.find(node) != node_set.end())
            {
                std::cout << "OK\n";
            }
            else
            {
                std::cout << "FAIL\n";
            }
        }
    }

    return 0;
}
