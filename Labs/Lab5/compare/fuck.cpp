#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAX_NODES = 100005;
const int MAX_EDGES = MAX_NODES * 20;
const int INF = 0x3f3f3f3f;

struct Edge {
    int start, end, cost, weight, id;
} graphEdges[MAX_EDGES], originalGraphEdges[MAX_EDGES];

struct EdgeReplacement {
    int oldEdgeId, newEdgeId;
} edgeReplacements[MAX_EDGES];

int parentNode[MAX_NODES], minIncomingCost[MAX_NODES], incomingEdgeId[MAX_NODES];
int componentId[MAX_NODES], visitStatus[MAX_NODES], activeEdges[MAX_EDGES];
int previousNode[MAX_NODES];

void insertEdge(int start, int end, int cost, int weight, int id) {
    graphEdges[id] = {start, end, cost, weight, id};
}

int computeMinimumCostTree(int root, int nodeCount, int edgeCount) {
    int totalWeight = 0, newEdgeIndex = edgeCount;
    fill(activeEdges, activeEdges + MAX_EDGES, 0);
    
    while (true) {
        fill(minIncomingCost, minIncomingCost + nodeCount, INF);
        
        for (int i = 0; i < edgeCount; i++) {
            if (graphEdges[i].start != graphEdges[i].end && graphEdges[i].cost < minIncomingCost[graphEdges[i].end]) {
                previousNode[graphEdges[i].end] = graphEdges[i].start;
                minIncomingCost[graphEdges[i].end] = graphEdges[i].cost;
                incomingEdgeId[graphEdges[i].end] = graphEdges[i].id;
            }
        }
        
        for (int i = 0; i < nodeCount; i++) {
            if (i != root && minIncomingCost[i] == INF) return -1;
        }
        
        int newComponentCount = 0;
        fill(componentId, componentId + nodeCount, -1);
        fill(visitStatus, visitStatus + nodeCount, -1);
        minIncomingCost[root] = 0;
        
        for (int i = 0; i < nodeCount; i++) {
            totalWeight += minIncomingCost[i];
            int node = i;
            
            if (i != root) activeEdges[incomingEdgeId[i]]++;
            
            while (visitStatus[node] != i && componentId[node] == -1 && node != root) {
                visitStatus[node] = i;
                node = previousNode[node];
            }
            
            if (node != root && componentId[node] == -1) {
                for (int u = previousNode[node]; u != node; u = previousNode[u]) {
                    componentId[u] = newComponentCount;
                }
                componentId[node] = newComponentCount++;
            }
        }
        
        if (newComponentCount == 0) break;
        
        for (int i = 0; i < nodeCount; i++) {
            if (componentId[i] == -1) componentId[i] = newComponentCount++;
        }
        
        for (int i = 0; i < edgeCount; i++) {
            int destination = graphEdges[i].end;
            graphEdges[i].start = componentId[graphEdges[i].start];
            graphEdges[i].end = componentId[graphEdges[i].end];
            
            if (graphEdges[i].start != graphEdges[i].end) {
                graphEdges[i].cost -= minIncomingCost[destination];
                edgeReplacements[newEdgeIndex] = {incomingEdgeId[destination], graphEdges[i].id};
                graphEdges[i].id = newEdgeIndex++;
            }
        }
        
        nodeCount = newComponentCount;
        root = componentId[root];
    }
    
    fill(parentNode, parentNode + MAX_NODES, -1);
    parentNode[root] = root;
    
    for (int i = newEdgeIndex - 1; i >= edgeCount; i--) {
        if (activeEdges[i]) {
            activeEdges[edgeReplacements[i].oldEdgeId]--;
            activeEdges[edgeReplacements[i].newEdgeId]++;
        }
    }
    
    for (int i = 0; i < edgeCount; i++) {
        if (activeEdges[i] && originalGraphEdges[i].weight) {
            parentNode[originalGraphEdges[i].end] = originalGraphEdges[i].start;
        }
    }
    
    return totalWeight;
}

int main() {
    int nodeCount, edgeCount;
    scanf("%d%d", &nodeCount, &edgeCount);
    
    for (int i = 0; i < edgeCount; i++) {
        int start, end, weight;
        scanf("%d%d%d", &start, &end, &weight);
        insertEdge(start, end, weight, weight, i);
        originalGraphEdges[i] = graphEdges[i];
    }
    
    int optimalCost = INF, optimalRoot = -1;
    int optimalParent[MAX_NODES];
    
    for (int root = 0; root < nodeCount; root++) {
        memcpy(graphEdges, originalGraphEdges, sizeof(originalGraphEdges));
        
        int currentCost = computeMinimumCostTree(root, nodeCount, edgeCount);
        
        if (currentCost != -1 && currentCost < optimalCost) {
            optimalCost = currentCost;
            optimalRoot = root;
            memcpy(optimalParent, parentNode, sizeof(parentNode));
        } else if (currentCost != -1 && currentCost == optimalCost && root < optimalRoot) {
            optimalRoot = root;
            memcpy(optimalParent, parentNode, sizeof(parentNode));
        }
    }
    
    if (optimalCost == INF) {
        cout << "impossible" << endl;
    } else {
        printf("%d %d\n", optimalCost, optimalRoot);
        for (int i = 0; i < nodeCount; i++) {
            if (i == optimalRoot) {
              cout << i << endl; // 根节点的父节点为-1
            } else {
                cout << i << " " << optimalParent[i] << endl;
            }
        }
    }
    
    return 0;
}