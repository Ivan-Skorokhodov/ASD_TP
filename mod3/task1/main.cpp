#include <iostream>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <stack>

void DFS(const IGraph &graph, int start_vertex, std::vector<bool> &visited, std::vector<int> &result)
{
    std::stack<int> stack;
    stack.push(start_vertex);

    while (!stack.empty())
    {
        int vertex = stack.top();
        stack.pop();

        if (!visited[vertex])
        {
            visited[vertex] = true;
            result.push_back(vertex);

            for (int next_vertex : graph.GetNextVertices(vertex))
            {
                if (!visited[next_vertex])
                {
                    stack.push(next_vertex);
                }
            }
        }
    }
}

int main()
{
    ListGraph graph(5);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(2, 3);
    graph.AddEdge(3, 4);

    std::vector<bool> visited0(graph.VerticesCount(), false);
    std::vector<int> result0;
    DFS(graph, 0, visited0, result0);

    std::cout << "DFS order ListGraph: ";
    for (int vertex : result0)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    MatrixGraph matrix_graph(graph);
    std::vector<bool> visited1(graph.VerticesCount(), false);
    std::vector<int> result1;
    DFS(matrix_graph, 0, visited1, result1);

    std::cout << "DFS order MatrixGraph: ";
    for (int vertex : result1)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    SetGraph set_graph(matrix_graph);
    std::vector<bool> visited2(graph.VerticesCount(), false);
    std::vector<int> result2;
    DFS(set_graph, 0, visited2, result2);

    std::cout << "DFS order SetGraph: ";
    for (int vertex : result2)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    ArcGraph arc_graph(set_graph);
    std::vector<bool> visited3(graph.VerticesCount(), false);
    std::vector<int> result3;
    DFS(arc_graph, 0, visited3, result3);

    std::cout << "DFS order ArcGraph: ";
    for (int vertex : result3)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}