#include "ListGraph.h"

ListGraph::ListGraph(int vertices_count)
{
    graph.resize(vertices_count);
    prev_graph.resize(vertices_count);
}

ListGraph::ListGraph(const IGraph &other)
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

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to)
{
    graph[from].push_back(to);
    prev_graph[to].push_back(from);
}

int ListGraph::VerticesCount() const
{
    return graph.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    return graph[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    return prev_graph[vertex];
}