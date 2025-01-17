#include <iostream>
#include <array>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

/*
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12]
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
Формат входных данных
Начальная расстановка.
Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек,
которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую последовательность ходов:
L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, то выведите любую из них.
Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.
*/

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishState = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState
{
public:
    GameState(const std::array<char, FieldSize> &field, int g = 0);

    bool CanMoveLeft() const;
    GameState MoveLeft() const;
    bool CanMoveUp() const;
    GameState MoveUp() const;
    bool CanMoveRight() const;
    GameState MoveRight() const;
    bool CanMoveDown() const;
    GameState MoveDown() const;

    bool IsFinishState() const;
    int Heuristic() const;

    char GetFieldValue(int index) const { return field.at(index); }
    bool operator==(const GameState &state) const { return field == state.field; }
    bool operator!=(const GameState &state) const { return !(*this == state); }
    bool operator<(const GameState &other) const
    {
        return this->Heuristic() < other.Heuristic();
    }

    int GetZeroPos() const { return zeroPos; }
    int GetCost() const { return g; }

private:
    std::array<char, FieldSize> field;
    int zeroPos;
    int g;
};

GameState::GameState(const std::array<char, FieldSize> &_field, int _g) : field(_field), g(_g), zeroPos(-1)
{
    for (int i = 0; i < FieldSize; ++i)
    {
        if (field[i] == 0)
        {
            zeroPos = i;
            break;
        }
    }
}

bool GameState::CanMoveLeft() const
{
    return zeroPos % SideSize > 0;
}

GameState GameState::MoveLeft() const
{
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - 1]);
    --newState.zeroPos;
    ++newState.g;
    return newState;
}

bool GameState::CanMoveUp() const
{
    return zeroPos >= SideSize;
}

GameState GameState::MoveUp() const
{
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    ++newState.g;
    return newState;
}

bool GameState::CanMoveRight() const
{
    return zeroPos % SideSize < SideSize - 1;
}

GameState GameState::MoveRight() const
{
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + 1]);
    ++newState.zeroPos;
    ++newState.g;
    return newState;
}

bool GameState::CanMoveDown() const
{
    return zeroPos < FieldSize - SideSize;
}

GameState GameState::MoveDown() const
{
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    ++newState.g;
    return newState;
}

bool GameState::IsFinishState() const
{
    return field == FinishState;
}

int GameState::Heuristic() const
{
    int h = 0;
    for (int i = 0; i < FieldSize; ++i)
    {
        if (field[i] != 0)
        {
            int targetX = (field[i] - 1) % SideSize;
            int targetY = (field[i] - 1) / SideSize;
            int currentX = i % SideSize;
            int currentY = i / SideSize;
            h += std::abs(targetX - currentX) + std::abs(targetY - currentY);
        }
    }
    return 2 * h;
}

struct StateHash
{
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        for (int i = 0; i < FieldSize; ++i)
        {
            hash = hash * 31 + state.GetFieldValue(i);
        }
        return hash;
    }
};

void tryMove(char move,
             bool (GameState::*canMove)() const,
             GameState (GameState::*moveFunc)() const,
             const GameState &current,
             std::priority_queue<std::pair<int, GameState>, std::vector<std::pair<int, GameState>>, std::greater<>> &openSet,
             std::unordered_map<GameState, char, StateHash> &cameFrom,
             std::unordered_map<GameState, int, StateHash> &costSoFar)
{
    if ((current.*canMove)())
    {
        GameState next = (current.*moveFunc)();
        int newCost = costSoFar[current] + 1;

        if (!costSoFar.count(next) || newCost < costSoFar[next])
        {
            costSoFar[next] = newCost;
            int priority = newCost + next.Heuristic();
            openSet.push({priority, next});
            cameFrom[next] = move;
        }
    }
}

std::string SolveGame(const GameState &startState)
{
    using PQElement = std::pair<int, GameState>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> openSet;
    std::unordered_map<GameState, char, StateHash> cameFrom;
    std::unordered_map<GameState, int, StateHash> costSoFar;

    openSet.push({startState.Heuristic(), startState});
    cameFrom[startState] = 'S';
    costSoFar[startState] = 0;

    while (!openSet.empty())
    {
        GameState current = openSet.top().second;
        openSet.pop();

        if (current.IsFinishState())
        {
            std::string result;
            while (cameFrom[current] != 'S')
            {
                result += cameFrom[current];
                switch (cameFrom[current])
                {
                case 'R':
                    current = current.MoveRight();
                    break;
                case 'D':
                    current = current.MoveDown();
                    break;
                case 'L':
                    current = current.MoveLeft();
                    break;
                case 'U':
                    current = current.MoveUp();
                    break;
                }
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

        tryMove('R', &GameState::CanMoveLeft, &GameState::MoveLeft, current, openSet, cameFrom, costSoFar);
        tryMove('D', &GameState::CanMoveUp, &GameState::MoveUp, current, openSet, cameFrom, costSoFar);
        tryMove('L', &GameState::CanMoveRight, &GameState::MoveRight, current, openSet, cameFrom, costSoFar);
        tryMove('U', &GameState::CanMoveDown, &GameState::MoveDown, current, openSet, cameFrom, costSoFar);
    }

    return "-1";
}

bool isEvenPermutation(const std::vector<int> &field)
{
    int inversions = 0;

    int zeroPos = 0;

    for (int i = 0; i < FieldSize; ++i)
    {
        if (field[i] == 0)
        {
            zeroPos = i;
        }
        for (int j = 0; j < i; ++j)
        {
            if (field[i] != 0 && field[j] != 0 && field[j] > field[i])
            {
                ++inversions;
            }
        }
    }

    int k = (zeroPos / SideSize) + 1;

    return (inversions + k) % 2 == 0;
}

int main()
{
    std::array<char, FieldSize> field;
    std::vector<int> fieldVec;
    int n;
    for (int i = 0; i < FieldSize; ++i)
    {
        std::cin >> n;
        field[i] = n;
        fieldVec.push_back(n);
    }
    GameState startState(field);

    if (!isEvenPermutation(fieldVec))
    {
        std::cout << -1 << std::endl;
        return 0;
    }

    std::string solution = SolveGame(startState);
    if (solution != "-1")
    {
        std::cout << solution.size() << std::endl;
        std::cout << solution << std::endl;
    }
    else
    {
        std::cout << -1 << std::endl;
    }
    return 0;
}
