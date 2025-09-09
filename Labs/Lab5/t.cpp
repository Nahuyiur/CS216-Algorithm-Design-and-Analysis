#include <iostream>
#include <stack>
using namespace std;

// 定义常量
const int MAX_NODE = 10005;  // 最大顶点数
const int MAX_EDGE = 200005; // 最大边数（包括虚根边）

// 定义边的结构体，用于左偏树
struct Edge {
    int dist;         // 左偏树中节点的距离（到最远叶节点的距离）
    int left, right;  // 左偏树的左右子节点索引
    int to, from;     // 边的终点和起点
    long long weight; // 边的权重
    long long tag;    // 延迟标记，用于批量更新权重
};

// 全局变量
Edge leftist_tree[MAX_EDGE + 2 * MAX_NODE]; // 存储所有边的数组（包括左偏树结构）
int edge_count = 0;                         // 当前边的计数
int father[2 * MAX_NODE];                   // 并查集数组，用于环检测和收缩
int root[2 * MAX_NODE];                     // 每个顶点的左偏树根索引
bool visited[2 * MAX_NODE];                 // 标记顶点是否被访问
int parent[MAX_NODE];                       // 记录每个顶点在 MDST 中的前继（父节点）

// 向下传播延迟标记，更新子节点的权重
void push_down_tag(int x) {
    if (leftist_tree[x].tag) { // 如果当前节点有延迟标记
        if (leftist_tree[x].left) { // 更新左子节点
            leftist_tree[leftist_tree[x].left].weight += leftist_tree[x].tag;
            leftist_tree[leftist_tree[x].left].tag += leftist_tree[x].tag;
        }
        if (leftist_tree[x].right) { // 更新右子节点
            leftist_tree[leftist_tree[x].right].weight += leftist_tree[x].tag;
            leftist_tree[leftist_tree[x].right].tag += leftist_tree[x].tag;
        }
        leftist_tree[x].tag = 0; // 清空当前节点的延迟标记
    }
}

// 创建一条新边并加入左偏树
int new_edge(int from, int to, long long weight) {
    edge_count++; // 边计数加 1
    leftist_tree[edge_count] = {0, 0, 0, to, from, weight, 0}; // 初始化边
    return edge_count; // 返回边的索引
}

// 合并两个左偏树，返回合并后的根索引
int merge(int x, int y) {
    if (!x || !y) return x + y; // 如果有一个为空，返回另一个
    push_down_tag(x); // 传播 x 的延迟标记
    push_down_tag(y); // 传播 y 的延迟标记
    // 确保 x 的权重小于 y 的权重（小根堆）
    if (leftist_tree[x].weight > leftist_tree[y].weight) {
        swap(x, y);
    }
    // 递归合并 x 的右子树和 y
    leftist_tree[x].right = merge(leftist_tree[x].right, y);
    // 保持左偏树的性质：左子树深度不小于右子树
    if (leftist_tree[leftist_tree[x].left].dist < leftist_tree[leftist_tree[x].right].dist) {
        swap(leftist_tree[x].left, leftist_tree[x].right);
    }
    // 更新 x 的距离（右子树深度加 1）
    leftist_tree[x].dist = leftist_tree[leftist_tree[x].right].dist + 1;
    return x; // 返回合并后的根
}

// 弹出左偏树的最小边（根节点），返回剩余部分的根
int pop(int x) {
    push_down_tag(x); // 传播延迟标记
    return merge(leftist_tree[x].left, leftist_tree[x].right); // 合并左右子树
}

// 并查集：查找顶点 x 的根（代表节点）
int find_father(int x) {
    if (x == father[x]) return x; // 如果 x 是根，返回 x
    return father[x] = find_father(father[x]); // 路径压缩，递归查找
}

int main() {
    int n, m;
    cin >> n >> m; // 输入顶点数 n 和边数 m

    long long sum_weight = 0, ans = 0; // sum_weight: 输入边权重和，ans: MDST 总权重
    // 读入输入边
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w; // 输入边的起点 u、终点 v 和权重 w
        sum_weight += w;    // 累加权重
        u++; v++;           // 将 0-based 转换为 1-based
        int edge_id = new_edge(u, v, w); // 创建新边
        root[v] = merge(root[v], edge_id); // 将边加入终点 v 的左偏树
    }

    int super_root = 0; // 虚根编号为 0
    // 从虚根到每个顶点添加边
    for (int i = 1; i <= n; ++i) {
        int edge_id = new_edge(0, i, sum_weight + 1); // 虚根到顶点 i 的边
        root[i] = merge(root[i], edge_id);
    }
    // 添加边 n -> 0（可能用于环检测）
    int edge_id = new_edge(n, 0, sum_weight + 1);
    root[0] = merge(root[0], edge_id);

    // 初始化并查集和 parent 数组
    for (int i = 0; i <= 2 * n; ++i) {
        father[i] = i; // 每个顶点初始为独立的集合
        parent[i] = -1; // 初始化前继为 -1（未分配）
    }

    // 使用栈进行图遍历和环检测
    stack<int> node_stack;
    int loop_count = n; // 用于超节点的编号，从 n 开始递增
    int real_root;      // 记录最终的根节点（经理）
    node_stack.push(super_root); // 从虚根开始
    visited[super_root] = true;  // 标记虚根已访问

    // 主循环：检测环并收缩，同时记录前继
    while (!node_stack.empty() && root[node_stack.top()]) {
        int current_node = node_stack.top(); // 当前处理的顶点
        int min_edge = root[current_node];   // 当前顶点的最小入边
        int from_node = find_father(leftist_tree[min_edge].from); // 入边的起点（并查集根）

        // 如果起点和终点相同（自环），弹出最小边
        if (from_node == current_node) {
            root[current_node] = pop(root[current_node]);
            continue;
        }

        // 如果起点未访问，加入栈中继续遍历
        if (!visited[from_node]) {
            visited[from_node] = true;
            node_stack.push(from_node);
            continue;
        }

        // 检测到环，进行收缩
        loop_count++; // 创建一个新的超节点
        while (visited[from_node]) {
            int node_id = node_stack.top(); // 弹出栈顶顶点
            node_stack.pop();
            visited[node_id] = false; // 取消访问标记
            father[node_id] = loop_count; // 将顶点加入超节点集合
            if (root[node_id]) { // 如果当前顶点有入边
                int edge = root[node_id]; // 获取最小入边
                int to = leftist_tree[edge].to; // 边的终点
                int from = leftist_tree[edge].from; // 边的起点
                // 更新权重并累加到总成本
                leftist_tree[root[node_id]].tag -= leftist_tree[root[node_id]].weight;
                if (find_father(leftist_tree[root[node_id]].to) != find_father(super_root)) {
                    ans += leftist_tree[root[node_id]].weight; // 累加边权重
                    if (from != super_root) { // 如果起点不是虚根，记录前继
                        parent[to] = from;
                    }
                }
                // 如果当前边是从虚根出发，记录根节点
                if (leftist_tree[root[node_id]].from == super_root) {
                    real_root = leftist_tree[root[node_id]].to;
                    parent[real_root] = real_root; // 根节点的前继是自身
                }
            }
            root[node_id] = pop(root[node_id]); // 弹出最小边
            root[loop_count] = merge(root[loop_count], root[node_id]); // 合并到超节点
        }
        visited[loop_count] = true; // 标记超节点已访问
        node_stack.push(loop_count); // 将超节点加入栈
    }

    // 输出结果
    if (ans > sum_weight * 2 + 1) {
        cout << "impossible" << endl; // 如果总成本过高，说明无解
    } else {
        // 输出实际总成本（减去虚根边权重）和根节点编号
        cout << ans - sum_weight - 1 << " " << real_root - 1 << endl;
        // 输出每个顶点及其前继
        for (int i = 1; i <= n; i++) {
            if (i == real_root) {
                cout << i - 1 << endl; // 根节点只输出自身编号（0-based）
            } else {
                if (parent[i] == -1) {
                    parent[i] = real_root; // 如果未分配前继，默认指向根节点
                }
                cout << i - 1 << " " << parent[i] - 1 << endl; // 输出节点和前继（0-based）
            }
        }
    }

    return 0;
}