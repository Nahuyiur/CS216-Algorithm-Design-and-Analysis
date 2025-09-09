#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

void floydWarshall(vector<vector<int> >& dist) {
    int V = dist.size();
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

int main() {
    int n, m, S;
    cin >> n >> m >> S;

    vector<vector<int>> graph(n, vector<int>(n, INT_MAX));

    // Initialize the diagonal to 0 (distance from a node to itself is 0)
    for (int i = 0; i < n; i++) {
        graph[i][i] = 0;
    }

    // Read the edges and update the graph
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;  // Convert to 0-based indexing
        graph[u][v] = w;
        graph[v][u] = w;  // Assuming undirected graph
    }

    // Build the distance matrix for Floyd-Warshall
    vector<vector<int>> dist = graph;

    // Compute all pairs shortest paths using Floyd-Warshall
    floydWarshall(dist);

    int src = 0;  // The first node (0-indexed)
    int dest = n - 1;  // The last node (0-indexed)
    
    int floydDist = dist[src][dest];


    cout << floydDist << endl;  // Output the shortest distance
    return 0;
}