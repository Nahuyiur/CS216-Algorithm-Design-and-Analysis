#include <complex>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAX_N = 3E6 + 5;
const double PI = acos(-1);

complex<double> F[MAX_N], G[MAX_N];
int reversed[MAX_N];

void generate_reversed(int len) { //位逆序置换
    for (int i = 0; i < len; i++) {
        reversed[i] = (reversed[i >> 1] >> 1) | ((i & 1) * (len >> 1));
        // 认为前一半已经算出来了，动态规划
        //(i & 1) 判断最低位是否是1，然后塞到最高位
    }
}

void fft(complex<double> *A, int len, int type) {
    for (int i = 0; i < len; i++) {
        if (i < reversed[i]) {
            swap(A[i], A[reversed[i]]);
        }//提前做好逆序重排
        //确保它们的顺序正好是递归拆分完的遍历顺序
    }

    for (int mid = 1; mid < len; mid <<= 1) { //第几层（长度为多少）的合并
        complex<double> Wn(cos(PI / mid), type * sin(PI / mid));
        for (int R = mid << 1, j = 0; j < len; j += R) { //对当前层的每一段进行处理（R是合并后的长度）
            complex<double> w(1, 0);
            for (int k = 0; k < mid; k++, w *= Wn) { //当前段内每一对蝴蝶操作
                complex<double> x = A[j + k], y = w * A[j + k + mid];
                A[j + k] = x + y;
                A[j + k + mid] = x - y;
            }
        }
    }

    if (type == -1) {
        for (int i = 0; i < len; i++) {
            A[i] /= len; //仅限逆FFT，做归一化
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i <= n; i++) {
        double coef;
        cin >> coef;
        F[i] = coef;
    }
    for (int i = 0; i <= m; i++) {
        double coef;
        cin >> coef;
        G[i] = coef;
    }

    int sum_len = n + m, binary_len = 1; // sum_len 是结果多项式的最大次数
    while (binary_len <= sum_len) { //binary_len 是 FFT 所需的长度（2 的幂），要 ≥ n + m + 1
        binary_len <<= 1;
    }

    generate_reversed(binary_len);
    fft(F, binary_len, 1); //最后一位表示方向
    fft(G, binary_len, 1);
    for (int i = 0; i < binary_len; i++) {
        F[i] *= G[i];
    }
    fft(F, binary_len, -1);
    for (int i = 0; i <= sum_len; i++) {
        cout << lround(F[i].real()) << " ";
    }
    return 0;
}