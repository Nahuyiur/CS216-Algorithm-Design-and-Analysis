#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

const int MAX_NODE = 200;
const int MAX_EDGE = 10200;
const int INF = 0x3f3f3f3f;

int source, target;                  // 超级源点和超级汇点
long long max_flow = 0, required_flow = 0;  // 当前最大流和需求总量

struct Edge {
    int to, next;   // 邻接点和下一条边的索引
    int value;      // 剩余容量
};

// 边数组（正反边成对存储）
Edge edges[MAX_EDGE * 2 + 2 + MAX_NODE * 2];

// 邻接表头数组
int head[MAX_NODE + 2];

// 边计数，从2开始方便用 i ^ 1 找反边
int edgeCount = 2;

// 当前弧优化数组，防止重复遍历
int cur[MAX_NODE + 2];

// 计算网络层次
int depth[MAX_NODE + 2];

// 节点需求（供需差）
int demand[MAX_NODE + 2];

// 记录边的下界，index对齐边编号
int lb[MAX_EDGE * 2 + 2 + MAX_NODE * 2];

// 添加边函数：包含正向边和反向边（容量0）
void add_edge(int from, int to, int capacity) {
    edges[edgeCount].to = to;
    edges[edgeCount].value = capacity;
    edges[edgeCount].next = head[from];
    head[from] = edgeCount;
    edgeCount++;

    edges[edgeCount].to = from;
    edges[edgeCount].value = 0;
    edges[edgeCount].next = head[to];
    head[to] = edgeCount;
    edgeCount++;
}

// BFS 构造分层图，判断是否存在增广路径
bool bfs() {
    memset(depth, 0x3f, sizeof(depth));
    queue<int> q;
    q.push(source);
    depth[source] = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 遍历当前节点所有邻接边
        for (int i = head[current]; i; i = edges[i].next) {
            int to = edges[i].to;

            // 边有剩余容量且终点未访问
            if (edges[i].value > 0 && depth[to] == INF) {
                depth[to] = depth[current] + 1;
                q.push(to);
            }
        }
    }
    // 如果汇点被访问到了，返回true
    return depth[target] != INF;
}

// DFS 寻找增广路径，返回可推送流量
int dfs(int now, int flow) {
    if (now == target) {
        max_flow += flow;
        return flow;
    }

    int flow_sum = 0;
    // 从当前弧开始遍历剩余边，减少重复遍历
    for (int &i = cur[now]; i; i = edges[i].next) {
        int v = edges[i].to;

        // 满足分层图条件且有剩余容量
        if (edges[i].value > 0 && depth[v] == depth[now] + 1) {
            int min_flow = dfs(v, min(flow - flow_sum, edges[i].value));

            if (min_flow > 0) {
                edges[i].value -= min_flow;
                edges[i ^ 1].value += min_flow;  // 更新反向边容量
                flow_sum += min_flow;
            }

            if (flow_sum == flow) break;
        }
    }
    return flow_sum;
}

// Dinic最大流主函数，循环构造分层图和寻找增广路径
void dinic() {
    while (bfs()) {
        memcpy(cur, head, sizeof(head));
        dfs(source, INF);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // 读边，计算需求并添加调整后的边
    for (int i = 0; i < m; i++) {
        int from, to, lower, upper;
        cin >> from >> to >> lower >> upper;

        // 计算节点供需差
        demand[from] -= lower;
        demand[to] += lower;

        // 记录边的下界，index 对齐边编号（正向边编号）
        lb[edgeCount] = lower;

        // 添加容量为上界减下界的边
        add_edge(from, to, upper - lower);
    }

    source = 0;
    target = n + 1;

    // 将节点需求转为超级源汇的边
    for (int i = 1; i <= n; i++) {
        if (demand[i] > 0) {
            add_edge(source, i, demand[i]);
            required_flow += demand[i];
        } else if (demand[i] < 0) {
            add_edge(i, target, -demand[i]);
        }
    }

    // 计算最大流
    dinic();

    // 判断是否满足总需求
    if (max_flow >= required_flow) {
        cout << "YES\n";

        // 输出每条边的实际流量 = 当前流 + 下界
        for (int i = 2; i < 2 * m + 2; i += 2) {
            cout << edges[i ^ 1].value + lb[i] << "\n";
        }
    } else {
        cout << "NO\n";
    }

    return 0;
}
