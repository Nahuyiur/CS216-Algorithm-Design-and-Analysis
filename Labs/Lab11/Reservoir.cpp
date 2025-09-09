#include <bits/stdc++.h>
using namespace std;

const int MAXN = 305;
const long long INF = 1e10;

int n;
long long capacity[MAXN][MAXN];  // 容量矩阵
long long flow[MAXN][MAXN];      // 实际流量（可以省略，也可以直接修改capacity）
int parent[MAXN];                // BFS中的父节点数组，用于回溯路径

// BFS 寻找从 s 到 t 的增广路径，并记录父节点
long long bfs(int s, int t) {
    fill(parent, parent + n, -1);  // 初始化父节点
    parent[s] = -2;  // 起点的父亲设为 -2 标记
    queue<pair<int, long long>> q;
    q.push({s, INF});  // 从源点开始，流量初始为INF

    while (!q.empty()) {
        int cur = q.front().first;
        long long cur_flow = q.front().second;
        q.pop();

        for (int next = 0; next < n; next++) {
            // 如果 next 没访问过，且存在剩余容量
            if (parent[next] == -1 && capacity[cur][next] > 0) {
                parent[next] = cur;
                long long new_flow = min(cur_flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;  // 找不到路径
}

long long max_flow(int s, int t) {
    long long total_flow = 0;

    while (true) {
        long long new_flow = bfs(s, t);
        if (new_flow == 0)
            break;  // 没有增广路径

        total_flow += new_flow;

        // 回溯路径并更新残量网络
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }
    return total_flow;
}

int main() {
    int m, s, t;
    cin >> n >> m >> s >> t;
    s--, t--;  // 转为 0-based 下标

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        capacity[u][v] += w;  // 如果有重边累加容量
    }

    cout << max_flow(s, t) << endl;
    return 0;
}
