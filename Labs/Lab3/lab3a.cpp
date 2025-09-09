#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<int> t(n); 
    vector<int> w(n); 
    for (int i = 0; i < n; i++) {
        cin >> t[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    
    vector<pair<int, int>> games(n); 
    for (int i = 0; i < n; i++) {
        games[i] = {w[i], t[i]};
    }
    sort(games.begin(), games.end(), greater<pair<int, int>>()); // 按惩罚金额降序
    
    int max_t = 0;
    for (int i = 0; i < n; i++) {
        max_t = max(max_t, t[i]);
    }
    // 记录每个时间段是否被占用
    vector<bool> used(max_t + 1, false);
    
    int total_penalty = 0; 
    for (int i = 0; i < n; i++) {
        int deadline = games[i].second; 
        int penalty = games[i].first;   
        bool scheduled = false;
        
        for (int j = deadline; j >= 1; j--) {
            if (!used[j]) {
                used[j] = true;
                scheduled = true;
                break;
            }
        }
        
        if (!scheduled) {
            total_penalty += penalty;
        }
    }
    
    cout << total_penalty << endl;
    return 0;
}