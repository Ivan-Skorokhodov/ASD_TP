#include "SetGraph.h"

SetGraph::SetGraph(int vertices_count)
{
    graph.resize(vertices_count);
    prev_graph.resize(vertices_count);
}

SetGraph::SetGraph(const IGraph &other)
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

SetGraph::~SetGraph() {}

void SetGraph::AddEdge(int from, int to)
{
    graph[from].insert(to);
    prev_graph[to].insert(from);
}

int SetGraph::VerticesCount() const
{
    return graph.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    std::vector<int> next_vertices;
    for (int next : graph[vertex])
    {
        next_vertices.push_back(next);
    }
    return next_vertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> prev_vertices;
    for (int prev : prev_graph[vertex])
    {
        prev_vertices.push_back(prev);
    }
    return prev_vertices;
}