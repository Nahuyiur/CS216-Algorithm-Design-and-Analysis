#include <iostream>
#include <vector>
#include <string>
#include <climits>
using namespace std;

const long long INF = LLONG_MAX / 2;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s, t;
    cin >> s >> t;
    int m = s.size();
    int n = t.size();
    int d_cost, r_cost, c_cost, i_cost, s_cost;
    cin >> d_cost >> r_cost >> c_cost >> i_cost >> s_cost;

    // dp[i][j]:把前i个字符的s变成前j个字符的t的最小代价
    vector<vector<long long>> dp(m + 1, vector<long long>(n + 1, INF));
    dp[0][0] = 0; //两个空字符串

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            // Delete operation
            if (i < m) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + d_cost);
            }

            // Copy operation
            if (i < m && j < n && s[i] == t[j]) {
                dp[i + 1][j + 1] = min(dp[i + 1][j + 1], dp[i][j] + c_cost);
            }

            // Replace operation
            if (i < m && j < n) {
                dp[i + 1][j + 1] = min(dp[i + 1][j + 1], dp[i][j] + r_cost);
            }

            // Insert operation
            if (j < n) {
                dp[i][j + 1] = min(dp[i][j + 1], dp[i][j] + i_cost);
            }

            // Swap operation
            if (i + 1 < m && j + 1 < n && s[i] == t[j + 1] && s[i + 1] == t[j]) {
                dp[i + 2][j + 2] = min(dp[i + 2][j + 2], dp[i][j] + s_cost);
            }

            // Finish operation
            if (j == n && i < m) {
                long long cost = dp[i][j] + (m - i) * d_cost - 1;
                dp[m][n] = min(dp[m][n], cost);
            }
        }
    }

    cout << dp[m][n] << endl;

    return 0;
}