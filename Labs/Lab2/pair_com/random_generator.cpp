#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
using namespace std;

int main() {
    srand(time(0));

    int N, M;

    // 随机生成N和M，保证它们是正的
    N = rand() % 100 + 1;  // 1到100之间的整数
    M = rand() % 200 + 1;  // 1到200之间的整数

    cout << N << " " << M << endl;

    // 随机生成 cap 数组，值可以是负数但不为0
    vector<int> cap(M);
    unordered_set<int> cap_set;  // 用于保证cap中的元素不重复
    for (int i = 0; i < M; i++) {
        int value;
        do {
            value = rand() % 10000 - 2000;  // 生成范围在[-5000, 4999]之间的随机数
        } while (value == 0 || cap_set.count(value)); // 避免0以及重复的值
        cap[i] = value;
        cap_set.insert(value);
        cout << cap[i] << " ";
    }
    cout << endl;

    // 随机生成 s 数组，值可以是负数但不为0，且所有元素不重复
    vector<vector<int> > s(N, vector<int>(M));
    unordered_set<int> s_set;  // 用于保证s数组中的元素不重复
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int value;
            do {
                value = rand() % 10000 - 2000;  // 生成范围在[-5000, 4999]之间的随机数
            } while (value == 0 || s_set.count(value)); // 避免0以及重复的值
            s[i][j] = value;
            s_set.insert(value);
            cout << s[i][j] << " ";
        }
        cout << endl;
    }

    // 随机生成 c 数组，值可以是负数但不为0，且所有元素不重复
    vector<vector<int> > c(M, vector<int>(N));
    unordered_set<int> c_set;  // 用于保证c数组中的元素不重复
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int value;
            do {
                value = rand() % 10000 - 5000;  // 生成范围在[-5000, 4999]之间的随机数
            } while (value == 0 || c_set.count(value)); // 避免0以及重复的值
            c[i][j] = value;
            c_set.insert(value);
            cout << c[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
