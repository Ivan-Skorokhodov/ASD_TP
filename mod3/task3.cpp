#include <iostream>
#include <vector>
#include <set>
#include <queue>

/*
Требуется отыскать самый выгодный маршрут между городами.
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Каждый город в очереди с приоритетом должен находиться в единственном экземпляре.
*/

bool Relax(int u, int v, int &weight, std::vector<int> &d, std::vector<int> &p)
{
    if (d[v] > d[u] + weight)
    {
        d[v] = d[u] + weight;
        p[v] = u;
        return true;
    }
    return false;
}

void Dijkstra(std::vector<std::vector<std::pair<int, int>>> &graph, int n, int start, int end)
{
    const int INF = 1e9;

    std::vector<int> d(n, INF);
    std::vector<int> p(n, -1);
    std::set<std::pair<int, int>> mySet;

    d[start] = 0;
    mySet.insert({0, start});

    while (!mySet.empty())
    {
        int v = mySet.begin()->second;
        mySet.erase(mySet.begin());

        for (auto to : graph[v])
        {
            int to_v = to.first;
            int weight = to.second;

            int prev_d_to = d[to_v];

            if (d[to_v] == INF)
            {
                d[to_v] = d[v] + weight;
                p[to_v] = v;
                mySet.insert({d[to_v], to_v});
            }
            else if (Relax(v, to_v, weight, d, p))
            {
                mySet.erase({prev_d_to, to_v});
                mySet.insert({d[to_v], to_v});
            }
        }
    }

    std::cout << d[end] << std::endl;
}
int main()
{
    int N, M;
    int u, v, w;

    std::cin >> N >> M;

    std::vector<std::vector<std::pair<int, int>>> graph(N);

    for (int i = 0; i < M; ++i)
    {
        std::cin >> u >> v >> w;
        if (u == v)
        {
            graph[u].push_back({v, w});
            continue;
        }

        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    int start, end;
    std::cin >> start >> end;

    if (start == end)
    {
        std::cout << "0" << std::endl;
        return 0;
    }

    Dijkstra(graph, N, start, end);

    return 0;
}