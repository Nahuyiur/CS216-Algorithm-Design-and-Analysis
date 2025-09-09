#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main() {
    long long n, m;
    cin >> n >> m;

    vector<string> pkgNames(m);
    vector<long long> reps(m);

    for(long long i = 0; i < m; i++) {
        cin >> pkgNames[i] >> reps[i];
    }

    long long totalScore = 0;

    for(long long i = 0; i < m; i++) {
        long long change = 0;

        // 遍历包裹名称，计算 'A' 和 'B' 的影响
        for(char c : pkgNames[i]) {
            change += (c == 'A') ? 1 : -1;
        }

        totalScore += change * reps[i];
    }

    if(totalScore < 0) {
        cout << -1 << endl;
        return 0;
    }

    long long minAdjustment = 0;
    long long accumulatedScore = 0;

    for(long long i = m - 1; i >= 0; i--) {
        long long change = 0;
        long long minScore =1;
        if (pkgNames[i].back() == 'B') minScore=-1;

        for(long long j = pkgNames[i].length() - 1; j >= 0; j--) {
            change += (pkgNames[i][j] == 'A') ? 1 : -1;

            if(change < minScore) {
                minScore = change;
            }
        }

        if(change < 0) {
            minAdjustment = min(minAdjustment, change * (reps[i] - 1) + minScore + accumulatedScore);
        } else {
            minAdjustment = min(minAdjustment, minScore + accumulatedScore);
        }

        accumulatedScore += change * reps[i];
    }

    cout << -minAdjustment - 1 << endl;
    return 0;
}
