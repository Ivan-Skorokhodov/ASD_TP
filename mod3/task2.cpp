#include <iostream>
#include <vector>
#include <queue>

/*
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
Найдите количество различных кратчайших путей между заданными вершинами.
Требования: сложность O(V+E).
*/

struct IGraph
{
    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const = 0;
    virtual ~IGraph() {}

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph
{
public:
    explicit ListGraph(int vertices_count)
    {
        graph.resize(vertices_count);
        prev_graph.resize(vertices_count);
    }

    ListGraph(const IGraph &other)
    {
        int vertices_count = other.VerticesCount();
        graph.resize(vertices_count);
        prev_graph.resize(vertices_count);

        for (int i = 0; i < vertices_count; ++i)
        {
            for (int next : other.GetNextVertices(i))
            {
                AddEdge(i, next);
            }
        }
    }

    virtual ~ListGraph() {}

    void AddEdge(int from, int to) override
    {
        graph[from].push_back(to);
        prev_graph[to].push_back(from);
    }

    int VerticesCount() const override
    {
        return graph.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        return graph[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        return prev_graph[vertex];
    }

private:
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> prev_graph;
};

void BFS(const IGraph &graph, int start_vertex, std::vector<int> &dist, std::vector<int> &amount_roads)
{
    std::queue<int> queue;
    queue.push(start_vertex);
    dist[start_vertex] = 0;
    amount_roads[start_vertex] = 1;

    while (!queue.empty())
    {
        int vertex = queue.front();
        queue.pop();

        for (int next_vertex : graph.GetNextVertices(vertex))
        {
            if (dist[next_vertex] == -1)
            {
                queue.push(next_vertex);
                dist[next_vertex] = dist[vertex] + 1;
                amount_roads[next_vertex] = amount_roads[vertex];
            }
            else if (dist[next_vertex] == (dist[vertex] + 1))
            {
                amount_roads[next_vertex] += amount_roads[vertex];
            }
        }
    }
}
int main()
{
    int v, v1, v2, n;
    std::cin >> v >> n;

    ListGraph graph(v);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> v1 >> v2;
        graph.AddEdge(v1, v2);
        graph.AddEdge(v2, v1);
    }

    std::cin >> v1 >> v2;

    std::vector<int> dist;
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        dist.push_back(-1);
    }

    std::vector<int> amount_roads;
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        amount_roads.push_back(0);
    }

    BFS(graph, v1, dist, amount_roads);
    std::cout << amount_roads[v2] << std::endl;

    return 0;
}
