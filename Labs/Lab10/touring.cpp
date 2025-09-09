#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

int GREY=1;
int BLACK=2;
int WHITE=0;
// 边的结构体，包含了从哪个节点到哪个节点的观光价值和成本
struct Edge {
    int from, to, value, cost;
};

// 定义常量：节点数量的上限和边数量的上限
const int MAX_NODE = 7000;
const int MAX_EDGE = 10000;

// 存储图的邻接表：每个节点对应一个边的集合
vector<Edge> edges[MAX_NODE + 1];

// 存储输入的边信息
Edge temp_edge[MAX_EDGE];

// 访问标记数组：0 表示未访问，1 表示正在访问，2 表示已访问
int visited[MAX_NODE + 1];

// 存储节点的距离，初始化为一个很大的数
double dist[MAX_NODE + 1];

// 用于标记是否找到负环
bool loop_flag;

// SPFA + DFS 实现，递归的深度优先搜索来检查环
void spfa_dfs(int now, double k) {
    visited[now] = GREY;  // 标记当前节点为正在访问
    for (int i = 0; i < edges[now].size(); i++) {
        int to = edges[now][i].to;
        // 计算当前边的权重：v_i - k * c_i，更新距离
        if (dist[to] > dist[now] + k * edges[now][i].cost - edges[now][i].value) {
            dist[to] = dist[now] + k * edges[now][i].cost - edges[now][i].value;
            if (visited[to] == WHITE) {
                spfa_dfs(to, k);  // 继续访问该节点
            } else if (visited[to] == GREY) {
                // 找到环：如果到达已在访问中的节点，说明有负环
                loop_flag = true;
                return;
            }
        }
        // 如果已经找到负环，直接退出
        if (loop_flag) {
            return;
        }
    }
    visited[now] = WHITE;  // 完成当前节点的访问，标记为已访问
}

// DFS 用于检查图中是否存在可达环
bool dfs(int now) {
    visited[now] = GREY;  // 标记为正在访问
    for (int i = 0; i < edges[now].size(); i++) {
        int to = edges[now][i].to; // 从当前节点 now 出发的第 i 条边的目标节点 to
        if (visited[to] == WHITE) {
            if (dfs(to)) {
                return true;  // 找到环
            }
        } else if (visited[to] == GREY) {
            return true;  // 找到环
        }
    }
    visited[now] = BLACK;  // 完成访问，标记为已访问
    return false;
}

int main() {
    int n, m;
    cin >> n >> m;  // 输入节点数 n 和边数 m

    // 输入所有边的起点、终点和观光价值
    for (int i = 0; i < m; i++) {
        cin >> temp_edge[i].from >> temp_edge[i].to >> temp_edge[i].value;
    }
    // 输入每条边的成本
    for (int i = 0; i < m; ++i) {
        cin >> temp_edge[i].cost;
    }

    // 将每条边添加到邻接表中
    for (int i = 0; i < m; i++) {
        edges[temp_edge[i].from].push_back({temp_edge[i].from, temp_edge[i].to, temp_edge[i].value, temp_edge[i].cost});
    }

    // 为了确保从节点 0 出发访问所有节点，加入一条从虚拟节点 0 到每个其他节点的边
    for (int i = 1; i <= n; i++) {
        edges[0].push_back({0, i, 1, 0});  // 这里的 value 为 1，cost 为 0，仅作为辅助边
    }

    // 使用 DFS 检查图中是否有可达环
    fill(visited, visited + MAX_NODE + 1, WHITE);//设置为全都未访问
    if (!dfs(0)) {
        cout << -1 << endl;  // 如果没有可达环，输出 -1
        return 0;
    }

    // 二分法查找最优的 k
    double left = 0, right = 200;  // 初始查找区间，k 的值范围设定
    while (fabs(left - right) > 1e-7) {  // 精度判断：当差值小于1e-7时退出
        // 每次二分法都重新初始化距离和访问标记
        fill(dist, dist + MAX_NODE + 1, 1E9);  // 初始化距离为很大的数
        dist[0] = 0;  // 源节点的距离初始化为0
        fill(visited, visited + MAX_NODE + 1, WHITE);  // 初始化访问状态
        loop_flag = false;  // 重置负环标记

        double mid = (left + right) / 2;  // 计算当前中间值
        spfa_dfs(0, mid);  // 以当前 mid 值为比率，进行 SPFA + DFS 判断负环
        if (!loop_flag) {
            // 如果没有找到负环，则说明当前 k 值过大，尝试减小 k
            right = mid;
        } else {
            // 如果找到了负环，则说明当前 k 值过小，尝试增大 k
            left = mid;
        }
    }

    // 输出最终的最大比值，保留一位小数
    cout << fixed << setprecision(1) << left << endl;

    return 0;
}
