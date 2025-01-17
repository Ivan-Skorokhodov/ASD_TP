#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int vertices_count)
{
    matrix.resize(vertices_count, std::vector<bool>(vertices_count, false));
}

MatrixGraph::MatrixGraph(const IGraph &other)
{
    int vertices_count = other.VerticesCount();
    matrix.resize(vertices_count, std::vector<bool>(vertices_count, false));

    for (int i = 0; i < vertices_count; ++i)
    {
        for (int next : other.GetNextVertices(i))
        {
            AddEdge(i, next);
        }
    }
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to)
{
    matrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
    return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    std::vector<int> next_vertices;
    for (int i = 0; i < matrix.size(); ++i)
    {
        if (matrix[vertex][i])
        {
            next_vertices.push_back(i);
        }
    }
    return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> prev_vertices;
    for (int i = 0; i < matrix.size(); ++i)
    {
        if (matrix[i][vertex])
        {
            prev_vertices.push_back(i);
        }
    }
    return prev_vertices;
}