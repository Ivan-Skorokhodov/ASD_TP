#include <iostream>
#include <vector>
#include <queue>
#include <set>

/*
Рику необходимо попасть на межвселенную конференцию. За каждую телепортацию он платит бутылками лимонада,
поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день.
Ему известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада),
учитывая, что телепортация не занимает времени, а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!
*/

using namespace std;

vector<int> BFS(const vector<vector<pair<int, int>>> &graph, int N, int start)
{
    vector<bool> visited(N, false);
    vector<int> distance(N, -1);
    queue<int> q;

    q.push(start);
    visited[start] = true;
    distance[start] = 0;

    while (!q.empty())
    {
        int vertex = q.front();
        q.pop();

        for (auto neighbor_pair : graph[vertex])
        {
            int neighbor = neighbor_pair.first;
            if (!visited[neighbor])
            {
                distance[neighbor] = distance[vertex] + 1;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    return distance;
}

bool Relax(int u, int v, int &weight, vector<int> &d, vector<int> &p)
{
    if (d[v] > d[u] + weight)
    {
        d[v] = d[u] + weight;
        p[v] = u;
        return true;
    }
    return false;
}

void Dijkstra(vector<vector<pair<int, int>>> &graph, int n, int start, int end)
{
    const int INF = 1e9;

    vector<int> d(n, INF);
    vector<int> p(n, -1);
    set<pair<int, int>> mySet;

    d[start] = 0;
    mySet.insert({0, start});

    while (!mySet.empty())
    {
        int v = mySet.begin()->second;
        mySet.erase(mySet.begin());

        for (auto to : graph[v])
        {
            int to_v = to.first;
            int weight = to.second;

            int prev_d_to = d[to_v];

            if (d[to_v] == INF)
            {
                d[to_v] = d[v] + weight;
                p[to_v] = v;
                mySet.insert({d[to_v], to_v});
            }
            else if (Relax(v, to_v, weight, d, p))
            {
                mySet.erase({prev_d_to, to_v});
                mySet.insert({d[to_v], to_v});
            }
        }
    }

    std::cout << d[end] << std::endl;
}

int main()
{
    int N, M, K, start, end;
    cin >> N >> M >> K >> start >> end;

    start--;
    end--;

    vector<vector<pair<int, int>>> graph(N);
    for (int i = 0; i < M; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        u--;
        v--;

        graph[u].push_back({v, w});
    }

    vector<int> distance = BFS(graph, N, start);

    if (distance[end] == -1 || distance[end] >= K)
    {
        cout << -1 << endl;
        return 0;
    }

    Dijkstra(graph, N, start, end);

    return 0;
}
.