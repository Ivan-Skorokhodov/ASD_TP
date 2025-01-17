#include "ArcGraph.h"

ArcGraph::ArcGraph(int vertices_count) : vertices_count(vertices_count) {}

ArcGraph::ArcGraph(const IGraph &other)
{
    vertices_count = other.VerticesCount();
    for (int i = 0; i < vertices_count; ++i)
    {
        for (int next : other.GetNextVertices(i))
        {
            AddEdge(i, next);
        }
    }
}

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to)
{
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const
{
    return vertices_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    std::vector<int> next_vertices;
    for (const auto &edge : edges)
    {
        if (edge.first == vertex)
        {
            next_vertices.push_back(edge.second);
        }
    }
    return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> prev_vertices;
    for (const auto &edge : edges)
    {
        if (edge.second == vertex)
        {
            prev_vertices.push_back(edge.first);
        }
    }
    return prev_vertices;
}