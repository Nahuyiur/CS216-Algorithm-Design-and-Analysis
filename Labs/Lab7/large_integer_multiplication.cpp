#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

// 宏定义简化代码书写
#define rep(i, a, b) for (int i = (a); i <= (b); i++)
#define lop(i, a, b) for (int i = (a); i < (b); i++)
#define dwn(i, a, b) for (int i = (a); i >= (b); i--)
#define ceil(a, b) (a + (b - 1)) / b
#define db double

constexpr int N = 4000010;
constexpr int M = 2000010;
const double PI = acos(-1), eps = 1e-8;

int T, n, m;

// 复数结构体：用于 FFT 运算
struct Complex {
    double x, y;

    Complex operator+(const Complex &t) const {
        return {x + t.x, y + t.y};
    }

    Complex operator-(const Complex &t) const {
        return {x - t.x, y - t.y};
    }

    Complex operator*(const Complex &t) const {
        return {
            x * t.x - y * t.y,
            x * t.y + y * t.x
        };
    }
} a[N], b[N];

int rev[N], bit, tot, res[N];

// 快速傅里叶变换（正变换 inv=1，逆变换 inv=-1）
void fft(Complex a[], int inv) {
    for (int i = 0; i < tot; i++) {
        if (i < rev[i])
            swap(a[i], a[rev[i]]);
    }

    for (int mid = 1; mid < tot; mid <<= 1) {
        auto w1 = Complex({cos(PI / mid), inv * sin(PI / mid)});
        for (int i = 0; i < tot; i += mid * 2) {
            auto wk = Complex({1, 0});
            for (int j = 0; j < mid; j++, wk = wk * w1) {
                auto x = a[i + j], y = wk * a[i + j + mid];
                a[i + j] = x + y;
                a[i + j + mid] = x - y;
            }
        }
    }
}

// 多项式乘法（FFT 主体）
void workFFT(int n, int m) {
    while ((1 << bit) < n + m + 1)
        bit++;
    
    tot = 1 << bit;

    for (int i = 0; i < tot; i++)
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));

    fft(a, 1);
    fft(b, 1);

    for (int i = 0; i < tot; i++)
        a[i] = a[i] * b[i];

    fft(a, -1);

    for (int i = 0; i <= n + m; i++)
        res[i] = (int)(a[i].x / tot + 0.5);  // 四舍五入取整
}

// 主函数：处理大整数乘法
int main() {
    string sa, sb;
    cin >> sa >> sb;

    n = sa.length() - 1;
    m = sb.length() - 1;

    // 倒序读入
    rep(i, 0, n)
        a[i].x = sa[n - i] - '0';
    
    rep(i, 0, m)
        b[i].x = sb[m - i] - '0';

    // 多项式乘法（FFT）
    workFFT(n, m);

    int top = n + m;
    int i = 0;

    // 处理进位
    while (i <= top) {
        if (res[i] >= 10) {
            res[i + 1] += res[i] / 10;
            top += (i == top);  // 增加位数
            res[i] %= 10;
        }
        i++;
    }

    // 去除前导 0（保留 0 本身）
    while (!res[top] && top > 0)
        top--;

    // 倒序输出结果
    dwn(i, top, 0)
        cout << res[i];
    
    return 0;
}
