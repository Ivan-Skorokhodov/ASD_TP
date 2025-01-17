#include <iostream>
#include <vector>
#include <stack>

/*
Проверить, является ли неориентированный граф деревом.
*/

using namespace std;

bool hasCycleDFS(const vector<vector<int>> &graph, int start_vertex, vector<bool> &visited)
{
    vector<int> parent(graph.size(), -1);
    stack<int> stack;
    stack.push(start_vertex);

    while (!stack.empty())
    {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex])
        {
            visited[vertex] = true;

            for (int next_vertex : graph[vertex])
            {
                if (!visited[next_vertex])
                {
                    stack.push(next_vertex);
                    parent[next_vertex] = vertex;
                }
                else if (parent[vertex] != next_vertex)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool isConnectedAndAcyclic(const vector<vector<int>> &graph, int N)
{
    vector<bool> visited(N, false);

    if (hasCycleDFS(graph, 0, visited))
    {
        return false;
    }

    for (bool v : visited)
    {
        if (!v)
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

    if (isConnectedAndAcyclic(graph, N))
    {
        cout << 1 << endl;
    }
    else
    {
        cout << 0 << endl;
    }

    return 0;
}
