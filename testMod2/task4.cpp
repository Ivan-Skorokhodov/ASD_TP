#include <iostream>
#include <stack>
#include <string>

template <class T, class C>
class AVLTree
{
public:
    AVLTree(const C &_cmp) : root(nullptr), cmp(_cmp) {}
    ~AVLTree();

    void Add(const T &key)
    {
        root = addReq(root, key);
    }

    bool Has(const T &key) const
    {
        Node *p = hasReq(root, key);
        if (p == nullptr)
        {
            return false;
        }
        return true;
    }

    T Next(const T &key) const
    {
        Node *result = nextReq(root, key, nullptr);
        if (result)
        {
            return result->key;
        }
        return key;
    }

    T Prev(const T &key) const
    {
        Node *result = prevReq(root, key, nullptr);
        if (result)
        {
            return result->key;
        }
        return key;
    }

    T findStat(const int stat) const
    {
        Node *p = findStatReq(root, stat, 0);
        return p->key;
    }

    bool Delete(const T &key)
    {
        if (this->Has(key))
        {
            root = deleteReq(root, key);
            return true;
        }
        return false;
    };

private:
    struct Node
    {
        T key;
        unsigned char height;
        int countElems;
        Node *left;
        Node *right;
        Node(const T &key) : left(nullptr), right(nullptr), key(key), height(1), countElems(1) {}
    };
    Node *root;
    C cmp;

    void deleteTree(Node *node);

    Node *hasReq(Node *p, const T &key) const
    {
        if (p == nullptr)
        {
            return nullptr;
        }
        if (cmp(key, p->key) == -1)
        {
            return hasReq(p->left, key);
        }
        else if (cmp(key, p->key) == 0)
        {
            return p;
        }
        return hasReq(p->right, key);
    }

    Node *nextReq(Node *p, const T &key, Node *best) const
    {
        if (p == nullptr)
            return best;
        if (cmp(key, p->key) < 0)
        {
            return nextReq(p->left, key, p);
        }
        return nextReq(p->right, key, best);
    }

    Node *prevReq(Node *p, const T &key, Node *best) const
    {
        if (p == nullptr)
            return best;
        if (cmp(key, p->key) > 0)
        {
            return prevReq(p->right, key, p);
        }
        return prevReq(p->left, key, best);
    }

    Node *findStatReq(Node *p, const int stat, int posPred) const
    {
        if (p == nullptr)
            return nullptr;

        int leftCount = (p->left != nullptr) ? p->left->countElems : 0;
        int pos = leftCount + 1 + posPred;

        if (stat == pos)
        {
            return p;
        }
        if (stat > pos)
        {
            return findStatReq(p->right, stat, pos);
        }
        else
        {
            return findStatReq(p->left, stat, posPred);
        }
    }

    Node *addReq(Node *p, const T &key)
    {
        if (p == nullptr)
        {
            return new Node(key);
        }
        if (cmp(key, p->key) == -1)
        {
            p->left = addReq(p->left, key);
        }
        else if (cmp(key, p->key == 1))
        {
            p->right = addReq(p->right, key);
        }
        return balance(p);
    }

    Node *deleteReq(Node *p, const T &key)
    {
        if (cmp(key, p->key) == -1)
        {
            p->left = deleteReq(p->left, key);
        }
        else if (cmp(key, p->key) == 1)
        {
            p->right = deleteReq(p->right, key);
        }
        else
        {
            Node *q = p->left;
            Node *r = p->right;
            delete p;
            if (r == nullptr)
            {
                return q;
            }
            Node *min = findmin(r);
            min->right = removemin(r);
            min->left = q;
            return balance(min);
        }
        return balance(p);
    }

    Node *balance(Node *p)
    {
        fixheight(p);
        if (bfactor(p) == 2)
        {
            if (bfactor(p->right) < 0)
            {
                p->right = rotateright(p->right);
            }
            return rotateleft(p);
        }
        if (bfactor(p) == -2)
        {
            if (bfactor(p->left) > 0)
            {
                p->left = rotateleft(p->left);
            }
            return rotateright(p);
        }
        return p;
    }

    unsigned char height(Node *p)
    {
        if (p == nullptr)
        {
            return 0;
        }
        return p->height;
    }

    int countElems(Node *p)
    {
        if (p == nullptr)
        {
            return 0;
        }
        return p->countElems;
    }

    void fixheight(Node *p)
    {
        unsigned char hl = height(p->left);
        unsigned char hr = height(p->right);
        p->height = std::max(hl, hr) + 1;

        int cl = countElems(p->left);
        int cr = countElems(p->right);
        p->countElems = cl + cr + 1;
    }

    int bfactor(Node *p)
    {
        return height(p->right) - height(p->left);
    }

    Node *rotateright(Node *p)
    {
        Node *q = p->left;
        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    Node *rotateleft(Node *q)
    {
        Node *p = q->right;
        q->right = p->left;
        p->left = q;
        fixheight(q);
        fixheight(p);
        return p;
    }

    Node *findmin(Node *p)
    {
        if (p->left != nullptr)
        {
            return findmin(p->left);
        }
        return p;
    }

    Node *removemin(Node *p)
    {
        if (p->left == nullptr)
            return p->right;
        p->left = removemin(p->left);
        return balance(p);
    }
};

template <class T, class C>
AVLTree<T, C>::~AVLTree()
{
    deleteTree(root);
}

template <class T, class C>
void AVLTree<T, C>::deleteTree(Node *node)
{
    if (node != nullptr)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

struct IntCmp
{
    int operator()(int l, int r) const
    {
        if (l < r)
        {
            return -1;
        }
        else if (l == r)
        {
            return 0;
        }
        return 1;
    }
};

int main()
{
    IntCmp cmp;
    AVLTree<int, IntCmp> tree(cmp);
    std::string operation;
    int key;

    while (std::cin >> operation >> key)
    {
        if (operation == "insert")
        {
            if (!tree.Has(key))
            {
                tree.Add(key);
            }
        }
        else if (operation == "exists")
        {
            if (tree.Has(key))
            {
                std::cout << "true\n";
            }
            else
            {
                std::cout << "false\n";
            }
        }
        else if (operation == "delete")
        {
            if (tree.Has(key))
            {
                tree.Delete(key);
            }
        }
        else if (operation == "next")
        {
            int res = tree.Next(key);
            if (res == key)
            {
                std::cout << "none\n";
            }
            else
            {
                std::cout << res << "\n";
            }
        }

        else if (operation == "prev")
        {
            int res = tree.Prev(key);
            if (res == key)
            {
                std::cout << "none\n";
            }
            else
            {
                std::cout << res << "\n";
            }
        }
    }

    return 0;
}