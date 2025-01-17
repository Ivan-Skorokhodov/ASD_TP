#include <iostream>
#include <stack>
template <class T, class C>
class AVLTree
{
public:
    AVLTree(const C &_cmp) : root(nullptr), cmp(_cmp) {}
    ~AVLTree();

    AVLTree(const AVLTree &) = delete;
    AVLTree(AVLTree &&) = delete;
    AVLTree &operator=(const AVLTree &) = delete;
    AVLTree &operator=(AVLTree &&) = delete;

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

    void PrintInOrder() const
    {
        std::stack<Node *> stack;
        Node *node = root;
        while (node != nullptr || !stack.empty())
        {
            while (node != nullptr)
            {
                stack.push(node);
                node = node->left;
            }
            node = stack.top();
            stack.pop();
            std::cout << node->key << " ";
            node = node->right;
        }
    }

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

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i)
    {
        int key, stat;
        std::cin >> key >> stat;

        if (key >= 0)
        {
            tree.Add(key);
        }
        else
        {
            tree.Delete(std::abs(key));
        }

        std::cout << tree.findStat(stat + 1) << std::endl;
    }

    return 0;
}