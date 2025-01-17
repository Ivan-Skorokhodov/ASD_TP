#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне 0..232 -1

*/

template <typename Key>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf)
            : leaf(leaf) {}

        ~Node()
        {
            for (Node *child : children)
            {
                delete child;
            }
        }

        bool leaf;
        std::vector<Key> keys;        // t-1, 2t-1, в корне от 1 до 2t-1
        std::vector<Node *> children; // 2t
        // children[0] <= keys[0] <= children[1] <= keys[1]...
    };

    BTree(const BTree &) = delete;
    BTree(BTree &&) = delete;
    BTree &operator=(const BTree &) = delete;
    BTree &operator=(BTree &&) = delete;

    BTree(size_t min_degree)
        : root(nullptr), t(min_degree)
    {
        assert(min_degree >= 2);
    }

    ~BTree()
    {
        if (root)
        {
            delete root;
        }
    }

    void insert(const Key &key)
    {
        if (!root)
        {
            root = new Node(true);
        }

        if (is_node_full(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            split(newRoot, 0);
            root = newRoot;
        }

        insert_non_full(root, key);
    }

    bool find(const Key &key)
    {
        return find_aux(root, key);
    }

    void print_levels() const
    {
        if (!root)
            return;

        std::queue<Node *> queue;
        queue.push(root);

        while (!queue.empty())
        {
            int level_size = queue.size();
            for (int i = 0; i < level_size; ++i)
            {
                Node *node = queue.front();
                queue.pop();

                for (const Key &key : node->keys)
                {
                    std::cout << key << " ";
                }

                if (!node->leaf)
                {
                    for (Node *child : node->children)
                    {
                        queue.push(child);
                    }
                }
            }
            std::cout << "\n";
        }
    }

private:
    bool is_node_full(Node *node)
    {
        return node->keys.size() == 2 * t - 1;
    }

    bool find_aux(Node *node, const Key &key)
    {
        int i = 0;
        while (i < node->keys.size() && node->keys[i] < key)
        {
            ++i;
        }

        if (i < node->keys.size() && node->keys[i] == key)
        {
            return true;
        }
        else if (node->leaf)
        {
            return false;
        }
        else
        {
            return find_aux(node->children[i], key); // children[i] <= key
        }
    }

    void split(Node *node, int i)
    {

        Node *oldNode = node->children[i];
        Node *newNodeLeft = new Node(oldNode->leaf);
        Node *newNodeRight = new Node(oldNode->leaf);

        int centerIndex = oldNode->keys.size() / 2;

        newNodeLeft->keys = std::vector<Key>(oldNode->keys.begin(), oldNode->keys.begin() + centerIndex);
        newNodeRight->keys = std::vector<Key>(oldNode->keys.begin() + centerIndex + 1, oldNode->keys.end());

        if (!oldNode->leaf)
        {
            newNodeLeft->children = std::vector<Node *>(oldNode->children.begin(), oldNode->children.begin() + centerIndex + 1);
            newNodeRight->children = std::vector<Node *>(oldNode->children.begin() + centerIndex + 1, oldNode->children.end());
        }

        node->keys.insert(node->keys.begin() + i, oldNode->keys[centerIndex]);

        node->children[i] = newNodeLeft;
        node->children.insert(node->children.begin() + i + 1, newNodeRight);

        oldNode->children = std::vector<Node *>();
        delete oldNode;
    }

    void insert_non_full(Node *node, const Key &key)
    {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && node->keys[pos] > key)
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && node->keys[pos] > key)
            {
                pos--;
            }

            if (is_node_full(node->children[pos + 1]))
            {
                split(node, pos + 1);
                if (node->keys[pos + 1] < key)
                {
                    pos++;
                }
            }

            insert_non_full(node->children[pos + 1], key);
        }
    }

    Node *root;
    size_t t; // minimum degree
};

int main()
{
    size_t n;
    std::cin >> n;

    BTree<int> tree(n);

    int key;
    while (std::cin >> key)
    {
        tree.insert(key);
    }

    tree.print_levels();

    return 0;
}
