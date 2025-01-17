#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include "IGraph.h"
#include <vector>
#include <utility>

class ArcGraph : public IGraph
{
public:
    explicit ArcGraph(int vertices_count);
    ArcGraph(const IGraph &other);
    virtual ~ArcGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int vertices_count;
    std::vector<std::pair<int, int>> edges;
};

#endif // ARCGRAPH_H