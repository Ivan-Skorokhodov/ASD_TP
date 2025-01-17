#include <iostream>
#include <vector>
#include <queue>

// Нужно проверить, является ли неориентированный граф эйлеровым.

using namespace std;

bool BFS(const vector<vector<int>> &graph, int N)
{
    vector<bool> visited(N, false);
    queue<int> q;

    q.push(0);
    visited[0] = true;

    while (!q.empty())
    {
        int vertex = q.front();
        q.pop();

        for (int neighbor : graph[vertex])
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            return false;
        }
    }

    return true;
}

bool isEuler(const vector<vector<int>> &graph, int N)
{
    if (!BFS(graph, N))
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        if (graph[i].size() % 2 != 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    int N, M;
    cin >> N >> M;

    vector<vector<int>> graph(N);
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    if (isEuler(graph, N))
    {
        cout << 1 << endl;
    }
    else
    {
        cout << 0 << endl;
    }

    return 0;
}
