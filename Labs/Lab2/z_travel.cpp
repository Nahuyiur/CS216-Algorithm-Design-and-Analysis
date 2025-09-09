#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<pair<int, int>> stable_matching(const vector<vector<int>>& w, vector<int>& alice_cities, vector<int>& bob_cities, char strategy, bool alice_region_high) {
    int n = alice_cities.size();
    vector<pair<int, int>> matches;
    unordered_map<int, int> alice_match, bob_match;

    // Alice 和 Bob 的偏好排序
    vector<vector<pair<int, int>>> alice_prefs(n), bob_prefs(n);

    for (int i = 0; i < n; ++i) {
        int alice_city = alice_cities[i];
        for (int j = 0; j < n; ++j) {
            int bob_city = bob_cities[j];
            // 动态计算 w 矩阵的索引
            int row = alice_region_high ? bob_city - 1 : alice_city - 1;
            int col = alice_region_high ? alice_city - n - 1 : bob_city - n - 1;
            int time = w[row][col];
            alice_prefs[i].emplace_back(time, bob_city); // Alice 的偏好列表
            bob_prefs[j].emplace_back(time, alice_city); // Bob 的偏好列表
        }
        
        // Alice 偏好排序
        if (strategy == 'I') {
            // Alice 希望找到尽量小的匹配时间（升序）
            sort(alice_prefs[i].begin(), alice_prefs[i].end());
        } else {
            // Alice 希望找到尽量大的匹配时间（降序）
            sort(alice_prefs[i].rbegin(), alice_prefs[i].rend());
        }
    }

    for (int j = 0; j < n; ++j) {
        // Bob 偏好排序
        if (strategy == 'I') {
            // Bob 希望找到尽量大的匹配时间（降序）
            sort(bob_prefs[j].rbegin(), bob_prefs[j].rend());
        } else {
            // Bob 希望找到尽量小的匹配时间（升序）
            sort(bob_prefs[j].begin(), bob_prefs[j].end());
        }
    }

    // Gale-Shapley 算法
    vector<int> next_pref(n, 0); // 记录每个 Alice 城市的下一个偏好索引
    vector<bool> alice_free(n, true); // 记录每个 Alice 城市是否空闲
    int free_count = n; // 空闲的 Alice 城市数量

    while (free_count > 0) {
        for (int i = 0; i < n; ++i) {
            if (!alice_free[i]) continue;

            int alice_city = alice_cities[i];
            if (next_pref[i] >= n) {
                alice_free[i] = false;
                free_count--;
                continue;
            }

            int bob_city = alice_prefs[i][next_pref[i]].second; // Alice 的下一个偏好 Bob 城市
            int travel_time = alice_prefs[i][next_pref[i]].first; // 旅行时间
            next_pref[i]++;

            if (bob_match.find(bob_city) == bob_match.end()) {
                // Bob 未匹配，直接匹配
                alice_match[alice_city] = bob_city;
                bob_match[bob_city] = alice_city;
                alice_free[i] = false;
                free_count--;
            } else {
                // Bob 已匹配，检查当前 Alice 是否比 Bob 的当前匹配更优
                int current_alice = bob_match[bob_city];
                int row = alice_region_high ? bob_city - 1 : current_alice - 1;
                int col = alice_region_high ? current_alice - n - 1 : bob_city - n - 1;
                int current_time = w[row][col];
                if ((strategy == 'I' && travel_time > current_time) || (strategy == 'D' && travel_time < current_time)) {
                    // 当前 Alice 更优，替换匹配
                    alice_match.erase(current_alice);
                    alice_free[find(alice_cities.begin(), alice_cities.end(), current_alice) - alice_cities.begin()] = true;
                    alice_match[alice_city] = bob_city;
                    bob_match[bob_city] = alice_city;
                    alice_free[i] = false;
                }
            }
        }
    }

    // 生成匹配结果
    for (auto& match : alice_match) {
        matches.emplace_back(match.first, match.second);
    }

    return matches;
}

int main() {
    int k;
    cin >> k;
    while (k--) {
        int n;
        cin >> n;

        vector<vector<int>> w(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> w[i][j];
            }
        }

        cout << 'B' << endl;
        cout.flush();

        char alice_strategy;
        int alice_initial_city;
        cin >> alice_strategy >> alice_initial_city;

        // 判断 Alice 的区域
        bool alice_region_high = (alice_initial_city > n);

        vector<int> alice_region_cities, bob_region_cities;
        if (alice_region_high) {
            // Alice 的区域是 n+1 到 2n，Bob 的区域是 1 到 n
            for (int i = 0; i < n; ++i) {
                alice_region_cities.push_back(n + i + 1);
                bob_region_cities.push_back(i + 1);
            }
        } else {
            // Alice 的区域是 1 到 n，Bob 的区域是 n+1 到 2n
            for (int i = 0; i < n; ++i) {
                alice_region_cities.push_back(i + 1);
                bob_region_cities.push_back(n + i + 1);
            }
        }
        
        vector<pair<int, int>> matches = stable_matching(w, alice_region_cities, bob_region_cities, alice_strategy, alice_region_high);
        
        unordered_map<int, int> city_pair;
        for (auto& match : matches) {
            city_pair[match.first] = match.second;
            city_pair[match.second] = match.first;
        }
        
        int current_city = alice_initial_city;
        bool bob_turn = true;

        while (true) {
            if (!bob_turn) {
                int alice_move;
                cin >> alice_move;
                if (alice_move == -1) break;
                if (alice_move == -2) return 0;
                current_city = alice_move;
            } else {
                if (city_pair.find(current_city) == city_pair.end()) {
                    cout << -1 << endl;
                    cout.flush();
                    break;
                }
                int bob_move = city_pair[current_city];
                cout << bob_move << endl;
                cout.flush();
                current_city = bob_move;
            }
            bob_turn = !bob_turn;
        }
    }
    return 0;
}