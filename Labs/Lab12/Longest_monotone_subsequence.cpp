#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int INF = 0x3f3f3f3f;
const int MAXN = 205;  // n <= 100，节点最多 2n+2

// ---------- 网络流的边结构 ----------
struct Edge {
    int to, rev; // rev 是反向边编号
    int cap;     // 当前边的剩余容量
};

vector<Edge> graph[MAXN * 2]; // 邻接表图
int level[MAXN * 2];          // BFS 层数数组
int iter[MAXN * 2];           // 当前弧优化

// 添加一条从 from 到 to 的容量为 cap 的边，同时加上反向边
void add_edge(int from, int to, int cap) {
    graph[from].push_back({to, (int)graph[to].size(), cap});
    graph[to].push_back({from, (int)graph[from].size() - 1, 0});
}

// ---------- 构建分层图 ----------
void bfs(int s) {
    memset(level, -1, sizeof(level));
    queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const Edge &e : graph[u]) {
            if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }
}

// ---------- DFS 找增广路 ----------
int dfs(int u, int t, int flow) {
    if (u == t) return flow;
    for (int &i = iter[u]; i < graph[u].size(); ++i) {
        Edge &e = graph[u][i];
        if (e.cap > 0 && level[u] < level[e.to]) {
            int d = dfs(e.to, t, min(flow, e.cap));
            if (d > 0) {
                e.cap -= d;
                graph[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

// ---------- Dinic 主流程 ----------
int dinic(int s, int t) {
    int maxflow = 0;
    while (true) {
        bfs(s);
        if (level[t] < 0) break;  // 没路了
        memset(iter, 0, sizeof(iter));
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            maxflow += f;
        }
    }
    return maxflow;
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n + 1), x(n + 1), dp(n + 1);
    int S = 0;  // 最长不下降子序列的长度

    for (int i = 1; i <= n; ++i) cin >> a[i];  // 数列
    for (int i = 1; i <= n; ++i) cin >> x[i];  // 每个数字最多可用几次

    // Step 1: 动态规划求 LIS 长度
    for (int i = 1; i <= n; ++i) {
        dp[i] = 1;
        for (int j = 1; j < i; ++j) {
            if (a[j] <= a[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        S = max(S, dp[i]);
    }

    cout << S << '\n';  // 输出 LIS 长度

    // Step 2: 建图
    int src = 0, dst = 2 * n + 1;

    for (int i = 1; i <= n; ++i) {
        // 每个点拆成 i 和 i+n，限制次数为 x[i]
        add_edge(i, i + n, x[i]);
    }

    for (int i = 1; i <= n; ++i) {
        // 所有 dp=1 的点为起点，从源点连边
        if (dp[i] == 1) {
            add_edge(src, i, INF);
        }

        // 所有 dp=S 的点为终点，向汇点连边
        if (dp[i] == S) {
            add_edge(i + n, dst, INF);
        }

        // 满足转移关系的 i、j 连边（j+n -> i）
        for (int j = 1; j < i; ++j) {
            if (a[j] <= a[i] && dp[j] + 1 == dp[i]) {
                add_edge(j + n, i, INF);
            }
        }
    }

    // Step 3: 求最大流（即为 LIS 的数量）
    cout << dinic(src, dst) << '\n';
    return 0;
}
