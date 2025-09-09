#include <iostream>
#include <cstring>
#include <queue>

using namespace std;
const int MAX_NODE = 202;
const int MAX_EDGE = 9999;
const int INF = 0x3f3f3f3f;
int source, target;
long long max_flow = 0, required_flow = 0;

struct Edge {
    int to, next, value;
};
Edge edges[MAX_EDGE * 2 + MAX_NODE * 2 + 10];

int head[MAX_NODE + 2], edgeCount = 2;
int cur[MAX_NODE + 2];
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

int depth[MAX_NODE + 2], demand[MAX_NODE + 2], lb[MAX_EDGE * 2 + 2 + MAX_NODE * 2];
bool bfs() {
    memset(depth, 0x3f, sizeof(depth));
    queue<int> q;
    q.push(source);
    depth[source] = 0;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int i = head[current]; i; i = edges[i].next) {
            int to = edges[i].to;
            if (edges[i].value > 0 && depth[to] == INF) {
                depth[to] = depth[current] + 1;
                q.push(to);
            }
        }
    }
    return depth[target] != INF;
}

int dfs(int now, int flow) {
    if (now == target) {
        max_flow += flow;
        return flow;
    }
    int flow_sum = 0;
    for (int &i = cur[now]; i; i = edges[i].next) {
        int v = edges[i].to;
        if (edges[i].value > 0 && depth[v] == depth[now] + 1) {
            int min_flow = dfs(v, min(flow - flow_sum, edges[i].value));
            if (min_flow > 0) {
                edges[i].value -= min_flow;
                edges[i ^ 1].value += min_flow;
                flow_sum += min_flow;
            }
            if (flow_sum == flow) {
                break;
            }
        }
    }
    return flow_sum;
}

void dinic() {
    while (bfs()) {
        memcpy(cur, head, sizeof(head));
        dfs(source, INF);
    }
}

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    for (int i = 0; i < m; i++) {
        int from, to, lower, upper;
        cin >> from >> to >> lower >> upper;
        demand[from] -= lower;
        demand[to] += lower;
        lb[edgeCount] = lower;
        add_edge(from, to, upper - lower);
    }
    source = 0;
    target = n + 1;
    for (int i = 1; i <= n; i++) {
        if (demand[i] > 0) {
            add_edge(source, i, demand[i]);
            required_flow += demand[i];
        } else if (demand[i] < 0) {
            add_edge(i, target, -demand[i]);
        }
    }
    add_edge(t, s, INF);

    dinic();


    //max_flow 表示当前网络（含超级源和超级汇）中，经过最大流算法计算得到的最大流量。
    //required_flow 表示 所有节点供需差中正需求部分的总和，也就是超级源点需要向网络“输入”的总流量。换句话说，它是把原图中所有“需要额外流入”的节点需求量加起来的总和。

    if (max_flow >= required_flow) {// 判断的是之前从超级源 source=0 到超级汇 target=n+1 的最大流，是否满足了所有节点的供需总需求（required_flow）。
        int ans = edges[edgeCount - 1].value;  // 记录回路边 t->s 的剩余容量
        max_flow = 0;                         // 重置最大流计数器
        source = s;                          // 恢复为原问题的源点
        target = t;                          // 恢复为原问题的汇点
        edges[edgeCount - 1].value = 0;     // 断开回路边 t->s 容量，防止流回环
        edges[edgeCount - 2].value = 0;     // 断开回路边的反向边容量
        dinic();                            // 重新计算 s 到 t 的最大流
        cout << max_flow + ans << endl;    // 输出真实的最大流（包含之前回路边上的流量）
    } else {
        cout << "please go home to sleep" << endl;
    }
    return 0;
}