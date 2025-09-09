#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

// 定义常量
const int MAX_NODES = 100005;         // 最大节点数
const int MAX_EDGES = MAX_NODES * 20; // 最大边数
const long INF = 0x3f3f3f3f3f3f3f3f;  // 无穷大
const int VIRTUAL_WEIGHT = 10000000;   // 虚拟边权重

// 定义边的结构体
struct Edge {
    int src, dst, cost, wgt, idx;
} edges[MAX_EDGES], initialEdges[MAX_EDGES];

// 定义边替换的结构体
struct EdgeReplace {
    int prevIdx, currIdx;
} edgeReplaces[MAX_EDGES];

// 全局变量
int nodeParent[MAX_NODES], minInCost[MAX_NODES], inEdgeIdx[MAX_NODES];
int cluster[MAX_NODES], visited[MAX_NODES], edgeUsed[MAX_EDGES];
int prevNode[MAX_NODES];

// 添加边到图中
void insertEdge(int src, int dst, int cost, int wgt, int idx) {
    edges[idx] = {src, dst, cost, wgt, idx};
}

// 计算最小树形图的总代价
long computeTreeCost(int rootNode, int nodeCount, int edgeCount) {
    long totalCost = 0;
    int nextEdgeIdx = edgeCount;
    fill(edgeUsed, edgeUsed + MAX_EDGES, 0);
    
    while (true) {
        fill(minInCost, minInCost + nodeCount, INF);
        
        // 寻找每个节点的最小入边
        for (int i = 0; i < edgeCount; i++) {
            if (edges[i].src != edges[i].dst && edges[i].cost < minInCost[edges[i].dst]) {
                prevNode[edges[i].dst] = edges[i].src;
                minInCost[edges[i].dst] = edges[i].cost;
                inEdgeIdx[edges[i].dst] = edges[i].idx;
            }
        }
        
        // 检查是否所有节点都有入边
        for (int i = 0; i < nodeCount; i++) {
            if (i != rootNode && minInCost[i] == INF) return -1;
        }
        
        int clusterCount = 0;
        fill(cluster, cluster + nodeCount, -1);
        fill(visited, visited + nodeCount, -1);
        minInCost[rootNode] = 0;
        
        // 计算总成本并检测环
        for (int i = 0; i < nodeCount; i++) {
            totalCost += minInCost[i];
            int currNode = i;
            
            if (i != rootNode) edgeUsed[inEdgeIdx[i]]++;
            
            while (visited[currNode] != i && cluster[currNode] == -1 && currNode != rootNode) {
                visited[currNode] = i;
                currNode = prevNode[currNode];
            }
            
            if (currNode != rootNode && cluster[currNode] == -1) {
                int tempNode = currNode;
                do {
                    cluster[tempNode] = clusterCount;
                    tempNode = prevNode[tempNode];
                } while (tempNode != currNode);
                clusterCount++;
            }
        }
        
        if (clusterCount == 0) {
            break;
        }
        
        // 分配未分组的节点
        for (int i = 0; i < nodeCount; i++) {
            if (cluster[i] == -1) cluster[i] = clusterCount++;
        }
        
        // 缩环并更新边
        for (int i = 0; i < edgeCount; i++) {
            int target = edges[i].dst;
            edges[i].src = cluster[edges[i].src];
            edges[i].dst = cluster[edges[i].dst];
            
            if (edges[i].src != edges[i].dst) {
                edges[i].cost -= minInCost[target];
                edgeReplaces[nextEdgeIdx] = {inEdgeIdx[target], edges[i].idx};
                edges[i].idx = nextEdgeIdx++;
            }
        }
        
        nodeCount = clusterCount;
        rootNode = cluster[rootNode];
    }
    
    // 构建父节点关系
    fill(nodeParent, nodeParent + MAX_NODES, -1);
    nodeParent[rootNode] = rootNode;
    
    for (int i = nextEdgeIdx - 1; i >= edgeCount; i--) {
        if (edgeUsed[i]) {
            edgeUsed[edgeReplaces[i].prevIdx]--;
            edgeUsed[edgeReplaces[i].currIdx]++;
        }
    }
    
    for (int i = 0; i < edgeCount; i++) {
        if (edgeUsed[i] && initialEdges[i].wgt) {
            int dd=initialEdges[i].dst;
            nodeParent[dd] = initialEdges[i].src;
        }
    }
    
    return totalCost;
}

int main() {
    int nNodes, nEdges;
    cin >> nNodes >> nEdges;
    
    int virtualNode = nNodes;
    int totalNodes = nNodes + 1;
    int totalEdges = nEdges + nNodes;
    
    // 输入边信息
    for (int i = 0; i < nEdges; i++) {
        int src, dst, wgt;
        cin >> src >> dst >> wgt;
        insertEdge(src, dst, wgt, wgt, i);
        initialEdges[i] = edges[i];
    }
    
    // 添加虚拟边
    for (int i = 0; i < nNodes; i++) {
        insertEdge(virtualNode, i, VIRTUAL_WEIGHT, VIRTUAL_WEIGHT, nEdges + i);
        initialEdges[nEdges + i] = edges[nEdges + i];
    }
    
    memcpy(edges, initialEdges, sizeof(initialEdges));
    long finalCost = computeTreeCost(virtualNode, totalNodes, totalEdges);
    
    // 输出结果
    if (finalCost == -1 || finalCost >= 2 * VIRTUAL_WEIGHT || finalCost < VIRTUAL_WEIGHT) {
        cout << "impossible" << endl;
    } else {
        int rootCount = 0;
        int treeRoot = -1;
        for (int i = 0; i < nNodes; i++) {
            if (nodeParent[i] == virtualNode) {
                rootCount++;
                treeRoot = i;
            }
        }
        if (rootCount != 1) {
            cout << "impossible" << endl;
        } else {
            long realCost = finalCost - VIRTUAL_WEIGHT;
            cout << realCost << " " << treeRoot << endl;
            for (int i = 0; i < nNodes; i++) {
                if (i == treeRoot) {
                    cout << i << endl;
                } else {
                    cout << i << " " << nodeParent[i] << endl;
                }
            }
        }
    }
    
    return 0;
}