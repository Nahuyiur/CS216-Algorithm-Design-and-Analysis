#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

typedef long long word;
typedef unsigned char byte;
const word INF = LLONG_MAX; // 无穷大，表示不可达

// 定义边的结构体
struct Edge {
    word from, to, cost, id; // 起点、终点、成本、边编号
};

// 全局变量
vector<Edge> edges; // 存储所有边
word n, m; // 节点数和边数
word minCost = INF; // 最小总成本
word bestRoot = -1; // 最佳根节点（经理）
vector<word> parent; // 记录每个节点的父节点（消息来源）
vector<bool> inTree; // 标记边是否在最小树形图中
vector<vector<pair<word, word>>> adj; // 邻接表，pair<目标节点, 边编号>

// 朱刘算法实现，返回最小树形图的成本
word chuLiu(word root) {
    vector<word> minEdge(n, INF); // 每个节点的最小入边权重
    vector<word> pre(n, -1);      // 最小入边的前驱节点
    vector<word> edgeId(n, -1);   // 最小入边的边ID
    vector<word> visited(n, -1);  // 访问标记，用于检测环
    vector<word> circle(n, -1);   // 节点所属的环ID

    // 第一步：选择最小入边
    for (word i = 0; i < m; ++i) {
        word u = edges[i].from, v = edges[i].to, cost = edges[i].cost;
        if (v != root && cost < minEdge[v]) {
            minEdge[v] = cost;
            pre[v] = u;
            edgeId[v] = i;
        }
    }

    // 检查是否所有节点都有入边（除根外）
    for (word i = 0; i < n; ++i) {
        if (i != root && minEdge[i] == INF) {
            return INF; // 存在不可达节点，无法形成树形图
        }
    }

    // 第二步：检测环
    word cnt = 0; // 环计数器
    for (word i = 0; i < n; ++i) {
        if (i == root || visited[i] != -1) continue;
        vector<word> path;
        word cur = i;
        while (visited[cur] == -1) {
            visited[cur] = i;
            path.push_back(cur);
            if (pre[cur] == -1) break;
            cur = pre[cur];
            if (visited[cur] == i) { // 发现环
                word circleId = cnt++;
                while (!path.empty() && path.back() != cur) {
                    circle[path.back()] = circleId;
                    path.pop_back();
                }
                circle[cur] = circleId;
                break;
            }
        }
    }

    // 如果没有环，直接返回结果
    if (cnt == 0) {
        word totalCost = 0;
        for (word i = 0; i < n; ++i) {
            if (i != root && pre[i] != -1) {
                totalCost += minEdge[i];
                parent[i] = pre[i];
                inTree[edgeId[i]] = true;
            }
        }
        return totalCost;
    }

    // 第三步：缩环并递归
    vector<Edge> newEdges;
    word totalCost = 0;
    for (word i = 0; i < n; ++i) {
        if (i != root && circle[i] == -1 && pre[i] != -1) {
            totalCost += minEdge[i];
            parent[i] = pre[i];
            inTree[edgeId[i]] = true;
        }
    }

    // 构建新图
    for (word i = 0; i < m; ++i) {
        word u = edges[i].from, v = edges[i].to, cost = edges[i].cost;
        if (u == root || v == root) continue;
        if (circle[u] == circle[v] && circle[u] != -1) continue; // 同一环内边丢弃
        word newCost = cost;
        if (circle[v] != -1) newCost -= minEdge[v]; // 调整权重
        word newU = (circle[u] == -1 ? u : n + circle[u]);
        word newV = (circle[v] == -1 ? v : n + circle[v]);
        newEdges.push_back({newU, newV, newCost, edges[i].id});
    }

    // 递归调用
    vector<word> newParent(n + cnt, -1);
    vector<bool> newInTree(newEdges.size(), false);
    swap(parent, newParent);
    swap(inTree, newInTree);
    vector<Edge> oldEdges = edges;
    edges = newEdges;
    m = edges.size();
    word subCost = chuLiu(root);
    if (subCost == INF) return INF;
    totalCost += subCost;
    swap(parent, newParent);
    swap(inTree, newInTree);
    edges = oldEdges;

    // 第四步：恢复环内的父节点
    for (word i = 0; i < n; ++i) {
        if (circle[i] != -1) {
            word newNode = n + circle[i];
            if (parent[newNode] != -1) {
                for (auto& e : adj[parent[newNode]]) {
                    if (e.first == i) {
                        parent[i] = parent[newNode];
                        inTree[e.second] = true;
                        break;
                    }
                }
                word cur = i;
                do {
                    word next = pre[cur];
                    parent[next] = cur;
                    inTree[edgeId[next]] = true;
                    cur = next;
                } while (cur != i);
            }
        }
    }

    return totalCost;
}

int main() {
    // 输入节点数和边数
    cin >> n >> m;
    edges.resize(m);
    adj.resize(n);
    parent.resize(n, -1);
    inTree.resize(m, false);

    // 输入边的信息
    for (word i = 0; i < m; ++i) {
        cin >> edges[i].from >> edges[i].to >> edges[i].cost;
        edges[i].id = i;
        adj[edges[i].from].push_back({edges[i].to, i});
    }

    // 尝试每个节点作为根节点
    for (word r = 0; r < n; ++r) {
        fill(parent.begin(), parent.end(), -1);
        fill(inTree.begin(), inTree.end(), false);
        word cost = chuLiu(r);
        if (cost < minCost) { // 更新最小成本和最佳根节点
            minCost = cost;
            bestRoot = r;
        }
    }

    // 输出结果
    if (bestRoot == -1) {
        cout << "impossible" << endl;
    } else {
        // 再次运行朱刘算法以确定最佳根节点的父节点关系
        fill(parent.begin(), parent.end(), -1);
        fill(inTree.begin(), inTree.end(), false);
        chuLiu(bestRoot);

        // 输出最小成本和经理编号
        cout << minCost << " " << bestRoot << endl;

        // 输出每个人的消息来源
        for (word i = 0; i < n; ++i) {
            if (i == bestRoot) {
                cout << i << endl; // 经理自己
            } else {
                cout << i << " " << parent[i] << endl; // 输出父节点
            }
        }
    }

    return 0;
}