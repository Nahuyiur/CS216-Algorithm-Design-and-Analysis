#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<vector>

using namespace std;

typedef long long ll;
const ll INF = 1e17;           // 无穷大，用于初始化最小入边权重
const int MAX_VERTICES = 1005; // 最大顶点数
const int MAX_EDGES = 20005;   // 最大边数

// 定义边的结构体，表示有向边
struct Edge {
    int from, to; // 边的起点和终点
    ll cost;      // 边的权重
};

Edge edges[MAX_EDGES];         // 存储所有边的数组
int predecessor[MAX_VERTICES]; // 记录每个顶点的最小入边来源（前驱顶点）
ll min_in_weight[MAX_VERTICES]; // 记录每个顶点的最小入边权重
int cycle_id[MAX_VERTICES];    // 标记每个顶点所属的环编号，用于环收缩
int visited[MAX_VERTICES];     // 在检测环时标记访问过的顶点
int root_edge_pos;             // 记录虚根到根节点的边的索引

// Edmonds 算法实现最小有向生成树
// virtual_root: 虚根编号
// num_vertices: 顶点总数（包括虚根）
// num_edges: 边总数（包括虚根边）
ll Directed_MST(int virtual_root, int num_vertices, int num_edges) {
    ll total_cost = 0; // MDST 的总权重

    while (true) {
        // 步骤 1：为每个顶点寻找最小入边
        for (int i = 0; i < num_vertices; i++) {
            min_in_weight[i] = INF; // 初始化最小入边权重为无穷大
            predecessor[i] = -1;    // 初始化前驱顶点为 -1（无前驱）
        }
        for (int i = 0; i < num_edges; i++) { // 遍历所有边（从 0 开始）
            int u = edges[i].from; // 边的起点
            int v = edges[i].to;   // 边的终点
            // 如果当前边的权重小于 v 的最小入边权重，且不是自环
            if (edges[i].cost < min_in_weight[v] && u != v) {
                predecessor[v] = u;           // 更新 v 的前驱为 u
                min_in_weight[v] = edges[i].cost; // 更新 v 的最小入边权重
                if (u == virtual_root) {
                    root_edge_pos = i; // 记录虚根到某节点的边索引
                }
            }
        }
        // 检查是否所有顶点（除虚根外）都有入边
        for (int i = 0; i < num_vertices; i++) {
            if (i == virtual_root) continue; // 跳过虚根
            if (min_in_weight[i] == INF) {
                return -1; // 存在顶点无法从根到达，MDST 不存在
            }
        }

        // 步骤 2：检测图中的环
        int num_cycles = 0; // 记录环的数量
        memset(cycle_id, -1, sizeof(cycle_id)); // 初始化环编号为 -1
        memset(visited, -1, sizeof(visited));   // 初始化访问标记为 -1
        min_in_weight[virtual_root] = 0;        // 虚根的最小入边权重设为 0
        for (int i = 0; i < num_vertices; i++) {
            total_cost += min_in_weight[i]; // 累加当前最小入边权重到总成本
            int v = i;
            // 沿着前驱路径寻找环
            while (visited[v] != i && cycle_id[v] == -1 && v != virtual_root) {
                visited[v] = i; // 标记 v 已被访问
                v = predecessor[v]; // 移动到前驱顶点
            }
            // 如果找到一个环（v 不是虚根且未被标记）
            if (v != virtual_root && cycle_id[v] == -1) {
                // 为环中的所有顶点分配相同的环编号
                for (int u = predecessor[v]; u != v; u = predecessor[u]) {
                    cycle_id[u] = num_cycles;
                }
                cycle_id[v] = num_cycles++;
            }
        }
        if (num_cycles == 0) break; // 如果没有环，退出循环

        // 为非环顶点分配唯一的编号
        for (int i = 0; i < num_vertices; i++) {
            if (cycle_id[i] == -1) {
                cycle_id[i] = num_cycles++; // 给非环顶点分配新编号
            }
        }

        // 步骤 3：收缩环并更新边
        for (int i = 0; i < num_edges; i++) {
            int v = edges[i].to;           // 原始终点
            edges[i].from = cycle_id[edges[i].from]; // 更新起点为环编号
            edges[i].to = cycle_id[edges[i].to];     // 更新终点为环编号
            if (edges[i].from != edges[i].to) {      // 如果起点和终点不在同一环
                edges[i].cost -= min_in_weight[v];   // 调整边的权重
            }
        }
        num_vertices = num_cycles;      // 更新顶点数为环的数量
        virtual_root = cycle_id[virtual_root]; // 更新虚根的编号
    }

    return total_cost; // 返回 MDST 的总权重
}

int main() {
    int num_vertices, num_input_edges;
    cin >> num_vertices >> num_input_edges; // 输入顶点数和边数

    ll total_edge_sum = 0; // 记录所有输入边的权重和
    for (int i = 0; i < num_input_edges; i++) { // 边从 0 开始
        scanf("%d%d%lld", &edges[i].from, &edges[i].to, &edges[i].cost); // 输入边的信息
        // 输入已经是 0-based，无需转换
        total_edge_sum += edges[i].cost; // 累加边的权重
    }

    total_edge_sum++; // 增加 1，用于虚根边的权重
    int virtual_root = num_vertices; // 虚根的编号为 num_vertices
    // 添加从虚根到每个顶点的边
    for (int i = 0, j = num_input_edges; i < num_vertices; i++, j++) {
        edges[j].from = virtual_root;    // 起点为虚根
        edges[j].to = i;               // 终点为顶点 i
        edges[j].cost = total_edge_sum; // 权重设为所有输入边权重和加 1
    }

    // 调用 Edmonds 算法计算 MDST
    ll result = Directed_MST(virtual_root, num_vertices + 1, num_input_edges + num_vertices);

    // 检查结果并输出
    if (result == -1 || result - total_edge_sum >= total_edge_sum) {
        printf("impossible\n\n"); // 如果 MDST 不存在或成本过高，输出 "impossible"
    } else {
        printf("%lld %d\n", result - total_edge_sum, edges[root_edge_pos].to); // 输出 MDST 成本和根节点（0-based）
        printf("\n");
    }

    return 0;
}