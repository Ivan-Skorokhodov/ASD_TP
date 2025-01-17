#include <iostream>
#include <stack>
#include <vector>

template <class T, class C>
class BinaryTree
{
public:
    BinaryTree(const C &_cmp) : root(nullptr), cmp(_cmp) {}
    ~BinaryTree();

    void Add(const T &key);
    bool Has(const T &key) const;
    bool Delete(const T &key);

    bool CheckInOrder() const;
    void CheckMinDepth(int *result) const
    {
        reqursiveCheckInOrder(root, 0, result, root);
    };

private:
    struct Node
    {
        Node *Left;
        Node *Right;
        T Key;
        Node(const T &key) : Left(nullptr), Right(nullptr), Key(key) {}
    };
    Node *root;
    C cmp;

    void reqursiveCheckInOrder(Node *node, int depth, int *result, Node *prev) const
    {
        if (node == nullptr && prev->Left == nullptr && prev->Right == nullptr)
        {
            if (depth < *result)
            {
                *result = depth;
            }
            return;
        }

        if (node == nullptr)
        {
            return;
        }
        reqursiveCheckInOrder(node->Left, depth + 1, result, node);
        reqursiveCheckInOrder(node->Right, depth + 1, result, node);
    };
};

template <class T, class C>
BinaryTree<T, C>::~BinaryTree()
{
    std::vector<Node *> list;

    std::stack<Node *> stack;
    Node *node = root;
    while (node != nullptr || !stack.empty())
    {
        while (node != nullptr)
        {
            stack.push(node);
            node = node->Left;
        }
        node = stack.top();
        stack.pop();
        list.push_back(node);
        node = node->Right;
    }

    for (int i = 0; i < list.size(); i++)
    {
        delete list[i];
    }
}

template <class T, class C>
void BinaryTree<T, C>::Add(const T &key)
{
    if (root == nullptr)
    {
        root = new Node(key);
        return;
    }

    Node *node = root;
    while ((cmp(key, node->Key) && node->Left != nullptr) || (!cmp(key, node->Key) && node->Right != nullptr))
    {
        if (cmp(key, node->Key))
        {
            node = node->Left;
        }
        else
        {
            node = node->Right;
        }
    }

    if (cmp(key, node->Key))
    {
        node->Left = new Node(key);
    }
    else
    {
        node->Right = new Node(key);
    }
}

template <class T, class C>
bool BinaryTree<T, C>::Has(const T &key) const
{
    if (root == nullptr)
    {
        return false;
    }

    Node *node = root;
    while (true)
    {
        if (node->Key == key)
        {
            return true;
        }

        if (cmp(key, node->Key))
        {
            if (node->Left == nullptr)
            {
                return false;
            }
            node = node->Left;
        }
        else
        {
            if (node->Right == nullptr)
            {
                return false;
            }
            node = node->Right;
        }
    }
}

template <class T, class C>
bool BinaryTree<T, C>::Delete(const T &key)
{
    if (root == nullptr)
    {
        return false;
    }

    if (root->Key == key)
    {
        Node *node = root;
        if (root->Left == nullptr && root->Right == nullptr)
        {
            root = nullptr;
        }
        else if (root->Left == nullptr)
        {
            root = node->Right;
        }
        else if (root->Right == nullptr)
        {
            root = node->Left;
        }
        else
        {
            Node *replacementNode = node->Right;
            Node *parentReplacementNode = node;

            while (replacementNode->Left != nullptr)
            {
                parentReplacementNode = replacementNode;
                replacementNode = replacementNode->Left;
            }

            if (parentReplacementNode != root)
            {
                parentReplacementNode->Left = replacementNode->Right;
                root = replacementNode;
                replacementNode->Left = node->Left;
                replacementNode->Right = node->Right;
            }
            else
            {
                root = replacementNode;
                replacementNode->Left = node->Left;
                replacementNode->Right = nullptr;
            }
        }

        delete node;
        return true;
    }

    Node *node = root;
    Node *parent = root;
    while (true)
    {
        if (node->Key == key)
        {
            break;
        }

        if (cmp(key, node->Key))
        {
            if (node->Left == nullptr)
            {
                return false;
            }
            parent = node;
            node = node->Left;
        }
        else
        {
            if (node->Right == nullptr)
            {
                return false;
            }
            parent = node;
            node = node->Right;
        }
    }

    if (cmp(key, parent->Key))
    {
        // left
        if (node->Left == nullptr && node->Right == nullptr)
        {
            parent->Left = nullptr;
        }
        else if (node->Left == nullptr)
        {
            parent->Left = node->Right;
        }
        else if (node->Right == nullptr)
        {
            parent->Left = node->Left;
        }
        else
        {
            Node *replacementNode = node->Right;
            Node *parentReplacementNode = node;

            while (replacementNode->Left != nullptr)
            {
                parentReplacementNode = replacementNode;
                replacementNode = replacementNode->Left;
            }

            parentReplacementNode->Left = replacementNode->Right;
            parent->Left = replacementNode;
            replacementNode->Left = node->Left;
            replacementNode->Right = node->Right;
        }
    }
    else
    {
        // right
        if (node->Left == nullptr && node->Right == nullptr)
        {
            parent->Right = nullptr;
        }
        else if (node->Left == nullptr)
        {
            parent->Right = node->Right;
        }
        else if (node->Right == nullptr)
        {
            parent->Right = node->Left;
        }
        else
        {
            Node *replacementNode = node->Right;
            Node *parentReplacementNode = node;

            while (replacementNode->Left != nullptr)
            {
                parentReplacementNode = replacementNode;
                replacementNode = replacementNode->Left;
            }

            parentReplacementNode->Left = replacementNode->Right;
            parent->Right = replacementNode;
            replacementNode->Left = node->Left;
            replacementNode->Right = node->Right;
        }
    }

    delete node;

    return true;
}

template <class T, class C>
bool BinaryTree<T, C>::CheckInOrder() const
{
    std::stack<Node *> stack;
    Node *node = root;
    while (node != nullptr || !stack.empty())
    {
        while (node != nullptr)
        {
            stack.push(node);
            node = node->Left;
        }
        node = stack.top();
        stack.pop();
        node = node->Right;
    }

    return true;
}

struct IntCmp
{
    bool operator()(int l, int r) const { return l < r; }
};

int main()
{
    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);

    int key;
    while (std::cin >> key)
    {
        tree.Add(key);
    }

    int result = 10000000;
    tree.CheckMinDepth(&result);
    std::cout << result << std::endl;

    return 0;
}