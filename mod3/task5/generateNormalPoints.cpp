#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>

/*
Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1.
Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения.
Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.

Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима.
*/

struct Point
{
    double x;
    double y;
};

std::vector<Point> generateNormalPoints(int count)
{
    double mean = 0.0;
    double stddev = 1.0;
    std::random_device rd;
    std::mt19937 gen(rd()); // Генератор случайных чисел (Mersenne Twister)
    std::normal_distribution<double> dist(mean, stddev);

    std::vector<Point> points;
    points.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        double x = dist(gen);
        double y = dist(gen);
        points.push_back({x, y});
    }

    return points;
}

double distance(const Point &a, const Point &b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double calculatePathLength(const std::vector<Point> &points, const std::vector<int> &order)
{
    double totalDistance = 0.0;
    for (size_t i = 0; i < order.size() - 1; ++i)
    {
        totalDistance += distance(points[order[i]], points[order[i + 1]]);
    }
    totalDistance += distance(points[order.back()], points[order[0]]);
    return totalDistance;
}

// Решение задачи коммивояжера
std::pair<double, std::vector<int>> solveTSP(const std::vector<Point> &points)
{
    int n = points.size();
    std::vector<int> order(n);
    for (int i = 0; i < n; ++i)
    {
        order[i] = i;
    }

    double minDistance = std::numeric_limits<double>::infinity();
    std::vector<int> bestOrder;

    do
    {
        double currentDistance = calculatePathLength(points, order);
        if (currentDistance < minDistance)
        {
            minDistance = currentDistance;
            bestOrder = order;
        }
    } while (std::next_permutation(order.begin(), order.end()));

    return {minDistance, bestOrder};
}

// Алгоритм Прима для решения задачи коммивояжера
std::vector<std::vector<int>> PrimMST(const std::vector<Point> &points)
{
    int n = points.size();
    std::vector<bool> visited(n, false);
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    std::vector<std::vector<int>> mst(n);

    pq.push({0.0, 0});

    while (!pq.empty())
    {
        auto [weight, u] = pq.top();
        pq.pop();

        if (visited[u])
        {
            continue;
        }

        visited[u] = true;

        for (int v = 0; v < n; ++v)
        {
            if (!visited[v] && u != v)
            {
                double edgeWeight = distance(points[u], points[v]);
                pq.push({edgeWeight, v});
                mst[u].push_back(v);
                mst[v].push_back(u);
            }
        }
    }

    return mst;
}

// Обход MST для построения пути с удалением повторов
std::vector<int> compressPath(const std::vector<int> &eulerPath)
{
    std::unordered_set<int> visited;
    std::vector<int> tspPath;
    for (int vertex : eulerPath)
    {
        if (visited.insert(vertex).second)
        {
            tspPath.push_back(vertex);
        }
    }
    return tspPath;
}

// Выполняет обход MST в порядке DFS и собирает Эйлеров путь
void dfs(int u, const std::vector<std::vector<int>> &mst, std::vector<bool> &visited, std::vector<int> &eulerPath)
{
    visited[u] = true;
    eulerPath.push_back(u);

    for (int v : mst[u])
    {
        if (!visited[v])
        {
            dfs(v, mst, visited, eulerPath);
            eulerPath.push_back(u);
        }
    }
}

// Решение задачи коммивояжера на основе Алгоритма Прима
std::vector<int> solvePrimTSP(const std::vector<Point> &points)
{
    auto mst = PrimMST(points);

    std::vector<bool> visited(points.size(), false);
    std::vector<int> eulerPath;
    dfs(0, mst, visited, eulerPath);

    std::vector<int> tspPath = compressPath(eulerPath);

    tspPath.push_back(0);

    return tspPath;
}

// Функция для выполнения эксперимента
void makeExperiment(int pointCount, double &approxCost, double &optimalCost)
{
    std::vector<Point> points = generateNormalPoints(pointCount);

    std::cout << "Generated points:" << std::endl;
    for (const auto &point : points)
    {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }
    std::cout << "-----------------" << std::endl;

    auto [minDistance, bestOrder] = solveTSP(points);

    std::cout << "Minimum path length: " << minDistance << std::endl;
    std::cout << "Optimal path: ";
    for (int index : bestOrder)
    {
        std::cout << index << " ";
    }
    std::cout << bestOrder[0] << std::endl;

    std::cout << "-----------------" << std::endl;

    std::vector<int> PrimOrder = solvePrimTSP(points);

    double PrimDistance = 0.0;
    std::cout << "Approximate TSP Path: ";
    for (size_t i = 0; i < PrimOrder.size(); ++i)
    {
        std::cout << PrimOrder[i] << " ";
        if (i > 0)
        {
            PrimDistance += distance(points[PrimOrder[i - 1]], points[PrimOrder[i]]);
        }
    }
    std::cout << std::endl;
    std::cout << "Total Distance: " << PrimDistance << std::endl;
    std::cout << "-----------------" << std::endl;

    approxCost = PrimDistance;
    optimalCost = minDistance;
}

// Функция для оценки качества алгоритма
void evaluate(int N, int amountExperiments, double &meanQualityOut, double &stdDevOut)
{
    std::vector<double> qualities;
    double approxCost, optimalCost;

    for (int t = 0; t < amountExperiments; ++t)
    {
        makeExperiment(N, approxCost, optimalCost);
        qualities.push_back(approxCost / optimalCost);
    }

    // Среднее значение
    double meanQuality = 0.0;
    for (double q : qualities)
    {
        meanQuality += q;
    }
    meanQuality /= amountExperiments;

    // Среднеквадратичное отклонение
    double variance = 0.0;
    for (double q : qualities)
    {
        variance += (q - meanQuality) * (q - meanQuality);
    }
    double stdDev = std::sqrt(variance / amountExperiments);

    std::cout << "N = " << N << ", amountExperiments = " << amountExperiments << "\n";
    std::cout << "Среднее значение: " << meanQuality << "\n";
    std::cout << "Среднеквадратичное отклонение: " << stdDev << "\n";

    meanQualityOut = meanQuality;
    stdDevOut = stdDev;
}

int main()
{
    int pointCount, amountExperiments;

    std::cout << "Enter amountExperiments: "; // ввести количество экспериментов при фиксированном N
    std::cin >> amountExperiments;

    std::vector<double> meanQualities;
    std::vector<double> stdDevs;

    double meanQuality = 0.0;
    double stdDev = 0.0;

    for (int i = 2; i < 9; ++i) // для измененения диапазона экспериментов при фиксированном N менять диапазон i тут
    {
        pointCount = i;
        evaluate(pointCount, amountExperiments, meanQuality, stdDev);

        meanQualities.push_back(meanQuality);
        stdDevs.push_back(stdDev);
    }

    std::cout << "\n==========================\n\n";
    std::cout << "Results of experiments:" << std::endl;

    for (int i = 2; i < 9; ++i)
    {
        std::cout << "N = " << i << ", amountExperiments = " << amountExperiments << "\n";
        std::cout << "Среднее значение: " << meanQualities[i - 2] << "\n";
        std::cout << "Среднеквадратичное отклонение: " << stdDevs[i - 2] << "\n\n";
    }

    std::cout << "\n==========================\n";

    return 0;
}
