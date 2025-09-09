#include <iostream>     // 包含输入输出流库
#include <cmath>        // 包含数学运算库（如 sqrt 函数）
#include <vector>       // 包含动态数组库
#include <algorithm>    // 包含排序和最小值函数
#include <climits>      // 包含长整型最大值常量
using namespace std;

const long long INF = LLONG_MAX;  // 定义一个很大的常数，表示无穷大，用于初始化最小距离

// 定义一个结构体来表示三维空间中的点，包含 x, y, z 坐标
struct Point {
    long long x, y, z;  // 使用 long long 防止坐标值过大导致溢出
};

// 比较函数：按 x 坐标排序，若 x 相等则按 y 再按 z 排序
bool comp_x(const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;  // 优先按 x 排序
    if (a.y != b.y) return a.y < b.y;  // 若 x 相等，按 y 排序
    return a.z < b.z;                  // 若 x, y 相等，按 z 排序
}

// 比较函数：按 y 坐标排序，若 y 相等则按 x 再按 z 排序
bool comp_y(const Point &a, const Point &b) {
    if (a.y != b.y) return a.y < b.y;  // 优先按 y 排序
    if (a.x != b.x) return a.x < b.x;  // 若 y 相等，按 x 排序
    return a.z < b.z;                  // 若 y, x 相等，按 z 排序
}

// 计算两点之间的平方欧几里得距离，避免直接开方以提高效率
long double dist(const Point &a, const Point &b) {
    long double dx = a.x - b.x;  // x 坐标差值
    long double dy = a.y - b.y;  // y 坐标差值
    long double dz = a.z - b.z;  // z 坐标差值
    return dx * dx + dy * dy + dz * dz;  // 返回平方距离
}

// 递归函数：使用分治法找出点集中最近点对的平方距离
long double closest_pair(vector<Point>& points, int left, int right) {
    // 如果区间为空，返回无穷大
    if (left >= right) return INF;
    // 如果只有两个点，直接计算它们之间的距离
    if (left + 1 == right) return dist(points[left], points[right]);

    // 计算中间点索引，用于分割点集
    int mid = (left + right) / 2;
    // 递归求解左半部分和右半部分的最近距离，取最小值
    long double min_dist = min(closest_pair(points, left, mid), closest_pair(points, mid + 1, right));

    // 创建一个条带（strip），存储 x 坐标接近中线点的集合
    vector<Point> strip;
    for (int i = left; i <= right; ++i) {
        long double dx = points[i].x - points[mid].x;  // 计算与中线 x 坐标的差值
        // 如果 x 方向的平方距离小于当前最小距离，加入条带
        if (dx * dx <= min_dist) {
            strip.push_back(points[i]);
        }
    }

    // 按 y 坐标对条带中的点排序，便于检查 y 方向的最近点
    sort(strip.begin(), strip.end(), comp_y);
    // 在条带中检查所有可能的点对
    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size(); ++j) {
            long double dy = strip[j].y - strip[i].y;  // y 坐标差值
            // 如果 y 方向的平方距离超过最小距离，跳出内层循环
            if (dy * dy > min_dist) break;
            // 计算当前点对的平方距离，更新最小距离
            min_dist = min(min_dist, dist(strip[i], strip[j]));
        }
    }
    // 返回当前区间内的最小平方距离
    return min_dist;
}

int main() {
    ios::sync_with_stdio(false);  // 关闭同步以加速 cin/cout
    cin.tie(nullptr);             // 解绑 cin 和 cout，提高输入效率

    int T; 
    cin >> T;
    while (T--) {  // 处理每个测试用例
        int n;  // 点数
        cin >> n;
        vector<Point> points(n);  // 创建存储点的向量
        // 输入每个点的坐标
        for (int i = 0; i < n; ++i) {
            cin >> points[i].x >> points[i].y >> points[i].z;
        }
        // 如果点数少于 2，距离为 0
        if (n < 2) {
            cout << "0.00\n";
            continue;
        }
        // 按 x 坐标排序，辅助分治
        sort(points.begin(), points.end(), comp_x);
        // 计算最近点对的平方距离，取平方根得到真实距离
        long double ans = sqrt(closest_pair(points, 0, n - 1));
        cout << fixed;          // 设置固定小数点输出
        cout.precision(2);      // 设置精度为两位小数
        cout << ans << '\n';    // 输出结果
    }
    return 0;
}