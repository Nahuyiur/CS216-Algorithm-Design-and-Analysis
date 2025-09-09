#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAX_NODES = 100005;
const int MAX_EDGES = MAX_NODES * 20;
const int INF = 0x3f3f3f3f;
const int VIRTUAL_EDGE_WEIGHT = 1e7; // 虚节点边的权值

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
    
    // 添加虚节点（编号为 nodeCount），并连接到所有其他节点，边权为 VIRTUAL_EDGE_WEIGHT
    int virtualNode = nodeCount;
    int totalNodes = nodeCount + 1;
    int totalEdges = edgeCount + nodeCount;
    
    for (int i = 0; i < edgeCount; i++) {
        int start, end, weight;
        scanf("%d%d%d", &start, &end, &weight);
        insertEdge(start, end, weight, weight, i);
        originalGraphEdges[i] = graphEdges[i];
    }
    
    // 添加从虚节点到其他所有节点的边
    for (int i = 0; i < nodeCount; i++) {
        insertEdge(virtualNode, i, VIRTUAL_EDGE_WEIGHT, VIRTUAL_EDGE_WEIGHT, edgeCount + i);
        originalGraphEdges[edgeCount + i] = graphEdges[edgeCount + i];
    }
    
    // 以虚节点为根计算最小树形图
    memcpy(graphEdges, originalGraphEdges, sizeof(originalGraphEdges));
    int totalCost = computeMinimumCostTree(virtualNode, totalNodes, totalEdges);
    
    if (totalCost == -1|totalCost >= 2*VIRTUAL_EDGE_WEIGHT|totalCost < VIRTUAL_EDGE_WEIGHT) {
        cout << "impossible" << endl;
    } else {
        // 总成本需要减去虚节点的一条边权（因为虚节点是根，必须选一条虚节点的边）
        int actualCost = totalCost - VIRTUAL_EDGE_WEIGHT;
        
        // 找到虚节点的子节点（即实际的根节点）
        int actualRoot = -1;
        for (int i = 0; i < nodeCount; i++) {
            if (parentNode[i] == virtualNode) {
                actualRoot = i;
                break;
            }
        }
        
        printf("%d %d\n", actualCost, actualRoot);
        for (int i = 0; i < nodeCount; i++) {
            if (i == actualRoot) {
                cout << i << endl; // 根节点的父节点为 -1
            } else {
                cout << i << " " << parentNode[i] << endl;
            }
        }
    }
    
    return 0;
}