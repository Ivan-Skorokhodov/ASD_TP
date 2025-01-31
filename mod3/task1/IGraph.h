#ifndef IGRAPH_H
#define IGRAPH_H

#include <vector>

class IGraph
{
public:
    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const = 0;
    virtual ~IGraph() {}

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

#endif // IGRAPH_H