#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool compareFiles(const string& file1, const string& file2) {
    ifstream f1(file1);
    ifstream f2(file2);
    string line1, line2;

    // 逐行比较
    while (getline(f1, line1) && getline(f2, line2)) {
        if (line1 != line2) {
            return false;
        }
    }
    // 检查是否都读到末尾，且无剩余内容
    bool f1_eof = f1.eof();
    bool f2_eof = f2.eof();
    return f1_eof && f2_eof && line1 == line2; // 确保两文件同时结束且最后一行相等
}

int main() {
    const int total_tests = 10;  // 总测试次数
    int passed = 0;              // 通过的测试次数
    int failed = 0;              // 失败的测试次数

    for (int t = 1; t <= total_tests; ++t) {
        // 生成测试数据
        system("./random_generator > data.txt");

        // 运行两个程序
        system("./fuck < data.txt > fuck.txt");
        system("./fuck2 < data.txt > fuck2.txt");

        // 比较文件内容
        if (!compareFiles("fuck.txt", "fuck2.txt")) {
            cout << "存在错误，测试案例 #" << t << ":" << endl;
            cout << "fuck.txt 内容:" << endl;
            ifstream f1("fuck.txt");
            string line;
            while (getline(f1, line)) {
                cout << line << endl;
            }
            cout << "fuck2.txt 内容:" << endl;
            ifstream f2("fuck2.txt");
            while (getline(f2, line)) {
                cout << line << endl;
            }
            cout << "-------------------" << endl;
            failed++;
        } else {
            passed++;
        }
    }

    // 输出总结
    cout << "总共测试 " << total_tests << " 个样例" << endl;
    if (failed == 0) {
        cout << "no error" << endl;
    } else {
        cout << "通过了 " << passed << " 个测试样例" << endl;
        cout << "发现了 " << failed << " 个错误" << endl;
    }

    return 0;
}