#include <iostream>
#include <vector>
#define int long long 
using namespace std;
const int INF = 1e8;

struct Edge{
    int from, to, length;
    Edge() : from(0), to(0), length(0){}
    Edge(int from, int to, int length):from(from), to(to), length(length){}
};

// dp[u][v][l][0]：表示从节点 u 到节点 v，在选择了 l 个子中心时，没有选择 u 为子中心，即所有的包裹都需要运送到 v
// dp[u][v][l][1]：表示从节点 u 到节点 v，在选择了 l 个子中心时，选择 u 为子中心，即部分包裹可以直接运送到 u，而不是运送到 v
void dfs(int u, int m, vector<int>& path, vector<vector<vector<vector<int>>>>& dp, vector<vector<Edge>> &edges, vector<int> &products, vector<int> &dist) {
    path.push_back(u);  // 将当前节点加入路径

    // 初始化
    for (int anc : path) { //anc是祖先节点
        for (int k = 0; k <= m; k++) {
            dp[u][anc][k][0] = 0;
            dp[u][anc][k][1] = 0;
        }
    }

    // 遍历所有子节点
    for (Edge& edge : edges[u]) {
        int v = edge.to;       // 子节点
        int length = edge.length;
        dist[v] = dist[u] + length;  // 更新子节点到根的距离
        dfs(v, m, path, dp, edges, products, dist);      // 递归处理子树

        // 合并子节点的状态
        for (int anc : path) {
            for (int k = m; k >= 0; k--) {
                // 不选 u 为子中心时，子节点 v 的快递运送到 anc
                dp[u][anc][k][0] += dp[v][anc][0][0];
                // 选 u 为子中心时，子节点 v 的快递运送到 u
                dp[u][anc][k][1] += dp[v][u][0][0];

                // 背包思想：分配 k 个子中心给子树
                for (int s = 0; s <= k; s++) {
                    dp[u][anc][k][0] = min(dp[u][anc][k][0], dp[u][anc][k - s][0] + dp[v][anc][s][0]);
                    dp[u][anc][k][1] = min(dp[u][anc][k][1], dp[u][anc][k - s][1] + dp[v][u][s][0]);
                }
            }
        }
    }

    // 处理当前节点 u 的快递成本
    for (int anc : path) {
        for (int k = 0; k <= m; k++) {
            if (k == 0) {
                // 必须运送到 anc
                dp[u][anc][k][0] += products[u] * (dist[u] - dist[anc]);
            } else {
                // 可选是否选 u 为子中心
                dp[u][anc][k][0] = min(
                    dp[u][anc][k][0] + products[u] * (dist[u] - dist[anc]),
                    dp[u][anc][k - 1][1]  // 选 u，占用 1 个子中心名额
                );
            }
        }
    }

    path.pop_back();  // 回溯，移除当前节点
}

signed main(){
    int n, m;
    cin >> n >> m;
    vector<vector<Edge>> edges(n + 1);
    vector<int> products(n + 1, 0);
    vector<int> dist(n + 1, 0);
    
    for(int i = 1; i <= n; i ++){
        int product, father, length;
        cin >> product >> father >> length;
        products[i] = product;
        edges[father].emplace_back(father, i, length);
    }

    vector<vector<vector<vector<int>>>> dp(
        n + 1,
        vector<vector<vector<int>>>(
            n + 1,
            vector<vector<int>>(
                m + 1,
                vector<int>(2, INF)  // INF 表示初始不可达
            )
        )
    );

    vector<int> path;  // 记录当前递归路径
    dfs(0, m, path, dp, edges, products, dist);  // 从根节点 0 开始

    cout << dp[0][0][m][0] << endl;  // 根节点必须选为分拣中心
    return 0;
    
}