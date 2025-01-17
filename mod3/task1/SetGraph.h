#ifndef SETGRAPH_H
#define SETGRAPH_H

#include "IGraph.h"
#include <vector>
#include <unordered_set>

class SetGraph : public IGraph
{
public:
    explicit SetGraph(int vertices_count);
    SetGraph(const IGraph &other);
    virtual ~SetGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> graph;
    std::vector<std::unordered_set<int>> prev_graph;
};

#endif // SETGRAPH_H