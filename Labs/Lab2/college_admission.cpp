#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

int main() {
    int N, M; // N 为学生数量，M 为大学数量
    cin >> N >> M;

    // 输入大学的容量，cap[i]表示第i个大学的最大招生人数
    vector<int> cap(M);
    for(int i = 0; i < M; i++) {
        cin >> cap[i];
    }

    // 输入学生对大学的满意度矩阵 s[i][j]，表示学生i对大学j的评价
    vector<vector<int>> s(N, vector<int>(M));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> s[i][j];
        }
    }

    // 输入大学对学生的评价矩阵 c[i][j]，表示大学i对学生j的评价
    vector<vector<int>> c(M, vector<int>(N));
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            cin >> c[i][j];
        }
    }

    // 构建学生的偏好列表，按学生的评价s[i][j]降序排序，去除负值评价
    vector<vector<int>> student_prefs(N); // 学生偏好列表，保存每个学生对大学的偏好顺序
    for(int i = 0; i < N; i++) {
        vector<pair<int, int>> prefs;
        for(int j = 0; j < M; j++) {
            if(s[i][j] > 0) {  // 如果学生对大学j的评价大于0
                prefs.push_back({j, s[i][j]});  // 保存大学j的编号和对应的评价
            }
        }
        // 按照评价降序排列学生的大学偏好
        sort(prefs.begin(), prefs.end(), [](pair<int, int>& a, pair<int, int>& b) {
            return a.second > b.second;
        });

        for(auto& p : prefs) {
            student_prefs[i].push_back(p.first);  // 将排好序的大学编号存入学生的偏好列表
        }
    }

    // 构建大学的偏好列表，按大学对学生的评价c[i][j]降序排序，去除负值评价
    vector<vector<int>> college_prefs(M); // 大学偏好列表，保存每个大学对学生的偏好顺序
    for(int i = 0; i < M; i++) {
        vector<pair<int, int>> prefs;
        for(int j = 0; j < N; j++) {
            if(c[i][j] > 0) {  // 如果大学对学生j的评价大于0
                prefs.push_back({j, c[i][j]});  // 保存学生j的编号和对应的评价
            }
        }
        // 按照评价降序排列大学的学生偏好
        sort(prefs.begin(), prefs.end(), [](pair<int, int>& a, pair<int, int>& b) {
            return a.second > b.second;
        });

        for(auto& p : prefs) {
            college_prefs[i].push_back(p.first);  // 将排好序的学生编号存入大学的偏好列表
        }
    }

    // 初始化每个大学的录取学生列表
    vector<vector<int>> enrolled(M);  // 记录每个大学已经录取的学生列表
    vector<int> free_students;  // 自由学生列表，记录没有被录取的学生
    for(size_t i = 0; i < N; i++) {
        free_students.push_back(i);  // 初始时所有学生都是自由的
    }

    // 每个学生的提议索引，记录学生在其偏好列表中申请的下一个大学
    vector<int> proposal_index(N, 0);  // 每个学生的提案索引，初始化为0;表示每个学生已经申请的大学数量

    // 实现 Gale-Shapley 算法（稳定匹配算法）
    while(!free_students.empty()) {  // 当还有自由学生时，继续循环
        bool made_proposal = false;  // 标记本轮是否有学生提出申请，初始为 false
        vector<int> temp_free = free_students;  // 复制 free_students，以便在循环中安全修改原列表

        for(int i : temp_free) {  // 遍历当前自由学生列表，i 是学生索引
            if(proposal_index[i] < (int)student_prefs[i].size()) {  // 检查学生 i 是否还有未申请的大学
                made_proposal = true;  // 有提案，标记为 true
                int college_j = student_prefs[i][proposal_index[i]];  // 获取学生 i 当前申请的大学索引
                proposal_index[i]++;  // 学生 i 的提案索引加 1，下次尝试下一个偏好

                // 大学处理提案
                if((int)enrolled[college_j].size() < cap[college_j] && c[college_j][i] > 0) {  // 大学未满且大学对学生 i 的评价 > 0
                    enrolled[college_j].push_back(i);  // 将学生 i 添加到大学 j 的录取列表
                    free_students.erase(remove(free_students.begin(), free_students.end(), i), free_students.end());  // 从自由学生列表中移除学生 i
                    //remove(free_students.begin(), free_students.end(), i) 会找到 free_students 中所有与 i 相等的学生，并将它们移动到容器的末尾。
                    //free_students.erase(...) 会删除从 remove 返回的迭代器到 free_students.end() 之间的所有元素，即移除学生 i。

                } else {  // 大学已满，需比较偏好
                    int least_preferred = -1;  // 记录最不受欢迎学生的索引，初始为 -1
                    int min_c = INT_MAX;  // 记录最低偏好分数，初始为最大整数
                    for(int student : enrolled[college_j]) {  // 遍历大学 j 当前录取的学生
                        if(c[college_j][student] < min_c) {  // 如果该学生的偏好分数低于当前最低值
                            min_c = c[college_j][student];  // 更新最低偏好分数
                            least_preferred = student;  // 更新最不受欢迎学生的索引
                        }
                    }
                    if(c[college_j][i] > min_c && s[i][college_j] > 0) {  // 如果学生 i 的偏好分数高于最低分数且学生 i 对该大学的评价 > 0
                        enrolled[college_j].erase(remove(enrolled[college_j].begin(), enrolled[college_j].end(), least_preferred), enrolled[college_j].end());  // 从大学 j 的录取列表中移除最不受欢迎的学生
                        enrolled[college_j].push_back(i);  // 将新学生 i 添加到大学 j 的录取列表
                        if(find(free_students.begin(), free_students.end(), least_preferred) == free_students.end()) {  // 检查被拒绝的学生是否已在自由列表
                            free_students.push_back(least_preferred);  // 如果不在，添加回自由学生列表
                        }
                        free_students.erase(remove(free_students.begin(), free_students.end(), i), free_students.end());  // 从自由学生列表中移除新录取的学生 i
                    }
                }
            }
        }

        if(!made_proposal) break;  // 如果本轮没有学生提出申请，结束循环
    }

    // 输出每个大学的录取情况
    for(int j = 0; j < M; j++) {
        cout << enrolled[j].size();
        for(int student : enrolled[j]) {
            cout << " " << student + 1;  // 输出 1-based 索引
        }
        cout << endl;
    }
    
    return 0;
}
