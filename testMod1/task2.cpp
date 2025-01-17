#include <iostream>
#include <string>
#include <cassert>

class Stack
{
public:
    explicit Stack(int _bufferSize);
    ~Stack();

    void Push(char a);
    char Pop();
    char Top() const { return buffer[top]; }
    bool IsEmpty() const { return top == -1; }

private:
    char *buffer;
    int bufferSize;
    int top;
};

Stack::Stack(int _bufferSize) : bufferSize(_bufferSize), top(-1)
{
    buffer = new char[bufferSize];
}

Stack::~Stack()
{
    delete[] buffer;
}

void Stack::Push(char a)
{
    assert(top + 1 < bufferSize);
    buffer[++top] = a;
}

char Stack::Pop()
{
    assert(top != -1);
    return buffer[top--];
}

bool isStackAnagram(const std::string &source, const std::string &target)
{
    if (source.length() != target.length())
        return false;

    Stack stack(source.length() + 1);
    std::string result;
    size_t targetIndex = 0;

    for (char ch : source)
    {
        stack.Push(ch);

        while (!stack.IsEmpty() && targetIndex < target.length() &&
               stack.Top() == target[targetIndex])
        {
            result += stack.Top();
            stack.Pop();
            targetIndex++;
        }
    }

    while (!stack.IsEmpty())
    {
        char top = stack.Top();
        stack.Pop();
        result += top;

        if (targetIndex >= target.length() || top != target[targetIndex])
        {
            return false;
        }
        targetIndex++;
    }

    return targetIndex == target.length();
}

int main()
{
    std::string source, target;
    std::cin >> source >> target;

    std::cout << (isStackAnagram(source, target) ? "YES" : "NO") << std::endl;

    return 0;
}
