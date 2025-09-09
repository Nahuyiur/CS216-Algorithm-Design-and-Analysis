#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 表示图中的一条边
struct Edge {
    int to;             // 边的终点
    int rev;            // 对应反向边在邻接表中的下标
    long long capacity; // 当前边的剩余容量
    long long cost;     // 单位流量的费用
};

class MinCostMaxFlow {
public:
    MinCostMaxFlow(int nodeCount, int source, int sink)
        : n(nodeCount), s(source), t(sink), maxFlow(0), minCost(0) {
        graph.resize(n);
        dist.resize(n);
        inQueue.resize(n);
        prevNode.resize(n);
        prevEdge.resize(n);
        visited.resize(n);
    }

    // 添加一条边及其对应的反向边
    void addEdge(int from, int to, long long capacity, long long cost) {
        graph[from].push_back({to, static_cast<int>(graph[to].size()), capacity, cost});
        graph[to].push_back({from, static_cast<int>(graph[from].size()) - 1, 0, -cost});
    }

    // 执行最小费用最大流主流程
    void run() {
        while (spfa()) { //只要还能找到费用最短的路径
            long long flow = dfs(s, numeric_limits<long long>::max());
            while (flow) { //只要还能继续推流，就继续推
                maxFlow += flow;
                minCost += flow * dist[t];
                flow = dfs(s, numeric_limits<long long>::max());
            }
        }
    }

    // 获取最大流量
    long long getMaxFlow() const {
        return maxFlow;
    }

    // 获取最小费用
    long long getMinCost() const {
        return minCost;
    }

private:
    int n; // 总节点数
    int s; // 源点
    int t; // 汇点
    vector<vector<Edge>> graph;  // 邻接表表示的图
    vector<long long> dist;      // 最短费用距离
    vector<bool> inQueue;        // SPFA中是否在队列
    vector<bool> visited;        // DFS中是否访问
    vector<int> prevNode;        // 增广路径前驱节点
    vector<int> prevEdge;        // 增广路径边索引
    long long maxFlow;           // 当前最大流
    long long minCost;           // 当前最小费用

    // 使用 SPFA 算法求最短费用路径
    bool spfa() { // 找单位费用最少 + 还能流的路径
        fill(dist.begin(), dist.end(), numeric_limits<long long>::max());
        fill(inQueue.begin(), inQueue.end(), false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        inQueue[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inQueue[u] = false;

            for (int i = 0; i < graph[u].size(); ++i) {
                Edge& e = graph[u][i];//从u出发的第i条边
                if (e.capacity > 0 && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    prevNode[e.to] = u;
                    prevEdge[e.to] = i;
                    if (!inQueue[e.to]) {
                        q.push(e.to);
                        inQueue[e.to] = true;
                    }
                }
            }
        }

        return dist[t] != numeric_limits<long long>::max();
    }

    // 在残量网络上 DFS 增广路径
    long long dfs(int u, long long flow) {
        if (u == t) return flow;

        visited[u] = true;
        long long totalPushed = 0;

        for (Edge& e : graph[u]) {
            if (e.capacity > 0 && !visited[e.to] && dist[e.to] == dist[u] + e.cost) {// 保证是最短路
                long long pushed = dfs(e.to, min(flow, e.capacity)); //返回值是本次成功推送的实际流量 pushed
                if (pushed > 0) {
                    e.capacity -= pushed;
                    graph[e.to][e.rev].capacity += pushed;
                    totalPushed += pushed;
                    flow -= pushed;
                    if (flow == 0) break;
                }
            }
        }

        visited[u] = false; //为了允许同一个节点在不同 DFS 分支中被复用，需要回溯时恢复状态
        return totalPushed;
    }
};

int main() {
    int n, m, source, sink;
    cin >> n >> m >> source >> sink;

    // 初始化最小费用最大流结构（节点编号建议从 1 开始）
    MinCostMaxFlow mcmf(n + 1, source, sink);  // 注意 n+1：支持编号从1到n

    // 读入所有边
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long capacity, cost;
        cin >> u >> v >> capacity >> cost;
        mcmf.addEdge(u, v, capacity, cost);
    }

    // 执行算法
    mcmf.run();

    // 输出最大流和最小费用
    cout << mcmf.getMaxFlow() << " " << mcmf.getMinCost() << endl;

    return 0;
}
