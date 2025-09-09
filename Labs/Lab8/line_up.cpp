#include<iostream>
using namespace std;
const int MOD=12345678;
const int B_MAX= 100;
const int G_MAX = 100;
const int K_MAX = 30;

long dp[B_MAX + 2][G_MAX + 2][K_MAX + 2][K_MAX + 2];
int main(){
    int n,m,k;
    cin>>n>>m>>k;

    dp[0][0][0][0]=1;

    //定义dp[i][j][b_cnt][g_cnt]表示前i个男孩，j个女孩，b_cnt个男孩，g_cnt个女孩的排列数
    // b_cnt表示当前男生比女生多的数量，g_cnt表示当前女生比男生多的数量
    for(int i=0;i<=n;i++){
        for(int j=0;j<=m;j++){
            for(int b_cnt=0;b_cnt<=k;b_cnt++){
                for(int g_cnt=0;g_cnt<=k;g_cnt++){
                    if(dp[i][j][b_cnt][g_cnt]!=0){
                        dp[i+1][j][b_cnt+1][max(g_cnt-1,0)]=(dp[i+1][j][b_cnt+1][max(g_cnt-1,0)]+dp[i][j][b_cnt][g_cnt])%MOD;
                        dp[i][j+1][max(b_cnt-1,0)][g_cnt+1]=(dp[i][j+1][max(b_cnt-1,0)][g_cnt+1]+dp[i][j][b_cnt][g_cnt])%MOD;
                    }
                }
            }
        }
    }

    long ans = 0;
    for (int i = 0; i <= k; i++) {
        for (int j = 0; j <= k; j++) {
            ans = (ans + dp[n][m][i][j]) % MOD;
        }
    }
    cout << ans << endl;
    return 0;
}