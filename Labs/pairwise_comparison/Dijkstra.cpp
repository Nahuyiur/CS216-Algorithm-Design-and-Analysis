#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
using namespace std;

vector<int> dijkstra(const vector<vector<pair<int, int> > >& adj, int src) {
    int V = adj.size();
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;
    vector<bool> visited(V, false);

    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main() {
    int n, m, S;
    cin >> n >> m >> S;

    vector<vector<pair<int, int>>> adj(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;  // Convert to 0-based indexing
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});  // Assuming undirected graph
    }

    int src = S - 1;  // Convert to 0-based indexing
    vector<int> dijkstraDist = dijkstra(adj, src);


    cout << dijkstraDist[n-1] <<endl;


    return 0;
}

