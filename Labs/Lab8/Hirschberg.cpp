#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 计算字符替换的代价，若相同为0，若不同为1
int mismatch(char x, char y) {
    return (x == y) ? 0 : 1;
}

// 计算插入或删除的代价，通常设为1
int gap(char c) {
    return 1;
}

// 计算两个字符串的编辑距离，利用动态规划优化空间
vector<int> DQ(const string &x, const string &y) {
    // 只保存两行来计算编辑距离
    vector<vector<int>> cost(2, vector<int>(y.size() + 1, 0));

    // 初始化第一行，表示将y的字符插入到空字符串中
    for (int i = 1; i <= y.size(); ++i) {
        cost[0][i] = cost[0][i - 1] + gap(y[i - 1]);
    }

    // 逐行计算编辑距离
    for (int i = 0; i < x.size(); ++i) {
        cost[1][0] = gap(x[i]) + cost[0][0];  // 处理第一列（字符插入到空字符串）

        for (int j = 1; j <= y.size(); ++j) {
            int cost1 = cost[0][j - 1] + mismatch(x[i], y[j - 1]);  // 替换代价
            int cost2 = cost[0][j] + gap(x[i]);  // 删除代价
            int cost3 = cost[1][j - 1] + gap(y[j - 1]);  // 插入代价

            // 选择最小的代价
            cost[1][j] = min({cost1, cost2, cost3});
        }

        // 将当前行替换为上一行，节省空间
        cost[0].swap(cost[1]);
    }

    // 返回计算出的最后一行，即从字符串x到y的最短编辑距离
    vector<int> ans(y.size());
    ans.swap(cost[0]);
    return ans;
}

// Hirschberg 算法，递归计算最短编辑距离，并返回最优匹配的字符串
vector<string> Hirschberg(const string &x, const string &y) {
    string a, b;
    vector<string> ans;

    // 递归终止条件：处理空字符串
    if (x.empty()) {
        for (int i = 0; i < y.size(); ++i) {
            a += "-";
            b += y[i];
        }
        ans.push_back(a);
        ans.push_back(b);
    }
    else if (y.empty()) {
        for (int i = 0; i < x.size(); ++i) {
            a += x[i];
            b += "-";
        }
        ans.push_back(a);
        ans.push_back(b);
    }
    // 如果其中一个字符串长度为1，直接计算相似性
    else if (x.size() == 1 || y.size() == 1) {
        ans = Similarity(x, y);
    }
    else {
        vector<int> cost1, cost2, cost;
        vector<string> ans1, ans2;
        string temp1, temp2, temp3, temp4, temp5;

        // 分割字符串x的前半部分
        temp1 = x.substr(0, x.size() / 2);
        temp2 = x.substr(x.size() / 2);

        // 反转字符串y，为了计算第二部分
        reverse(temp2.begin(), temp2.end());
        temp3 = y;
        reverse(temp3.begin(), temp3.end());

        // 计算前半部分和后半部分的编辑距离
        cost1 = DQ(temp1, y);
        cost2 = DQ(temp2, temp3);
        reverse(cost2.begin(), cost2.end());  // 恢复顺序

        // 合并计算成本
        for (int i = 0; i < cost1.size(); ++i) {
            cost.push_back(cost1[i] + cost2[i]);
        }

        // 找到最小的分割点
        int ymid = min_element(cost.begin(), cost.end()) - cost.begin();

        // 将y字符串分割为两部分
        temp4 = y.substr(0, ymid);
        temp5 = y.substr(ymid);

        // 递归计算左右子问题
        ans1 = Hirschberg(temp1, temp4);
        ans2 = Hirschberg(temp2, temp5);

        // 合并两部分的结果
        a = ans1[0] + ans2[0];
        b = ans1[1] + ans2[1];

        ans.push_back(a);
        ans.push_back(b);
    }

    return ans;
}

int main() {
    string x = "AGTACGCA";
    string y = "TATGC";
    
    vector<string> result = Hirschberg(x, y);

    cout << "Alignment result:" << endl;
    cout << result[0] << endl;
    cout << result[1] << endl;

    return 0;
}
