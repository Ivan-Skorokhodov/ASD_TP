#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "IGraph.h"
#include <vector>

class ListGraph : public IGraph
{
public:
    explicit ListGraph(int vertices_count);
    ListGraph(const IGraph &other);
    virtual ~ListGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> prev_graph;
};

#endif // LISTGRAPH_H