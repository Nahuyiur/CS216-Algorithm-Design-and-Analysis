#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 110;  // 字符串最大长度
const int INF = 1e9;   // 无穷大

char s[MAX_N];
int dp[MAX_N][MAX_N][2];  
// dp[l][r][0]：l到r没有B的最短长度
// dp[l][r][1]：l到r有B的最短长度

int n;

// 判断 s[l..r] 是否可以通过重复前半部分构造
bool isRepeat(int l, int r) {
    int length = r - l + 1;
    if (length % 2 != 0) return false; // 长度必须是偶数

    int mid = (l + r) / 2;
    for (int i = 0; i <= mid - l; i++) {
        if (s[l + i] != s[mid + 1 + i]) return false;
    }
    return true;
}

int main() {
    scanf("%s", &s[0] + 1);  // 从 s[1] 开始读入，方便下标
    n = strlen(s + 1);

    // 初始化：把所有 dp 值设为 "不可能的大值"
    for (int l = 1; l <= n; l++) {
        for (int r = l; r <= n; r++) {
            dp[l][r][0] = dp[l][r][1] = INF;
        }
    }

    // 初始化：单个字符，不折叠长度就是 1
    for (int i = 1; i <= n; i++) {
        dp[i][i][0] = dp[i][i][1] = 1;
    }

    // 枚举所有区间长度
    for (int length = 2; length <= n; length++) {
        for (int l = 1; l + length - 1 <= n; l++) {
            int r = l + length - 1;

            // ------- 不使用 R 的情况 -------

            // 1. 如果可以折叠（前半等于后半），使用一个 R
            if (isRepeat(l, r)) {
                // 只需要保留前半部分的压缩长度 + 1（一个 R）
                int mid = (l + r) / 2;
                dp[l][r][0] = min(dp[l][r][0], dp[l][mid][0] + 1);
            }

            // 2. 不折叠，拆成两部分
            for (int k = l; k < r; k++) {
                dp[l][r][0] = min(dp[l][r][0], dp[l][k][0] + (r - k));//前一半可能有R
            }

            // ------- 至少使用一个 R 的情况 -------

            // 尝试在任意位置把区间拆成两部分，并使用一个 R
            for (int k = l; k < r; k++) {
                dp[l][r][1] = min(dp[l][r][1],
                    min(dp[l][k][0], dp[l][k][1]) + 
                    min(dp[k + 1][r][0], dp[k + 1][r][1]) + 1
                );
            }
        }
    }

    // 答案：不使用 R 和使用至少一个 R 的情况取最小
    printf("%d\n", min(dp[1][n][0], dp[1][n][1]));

    return 0;
}
