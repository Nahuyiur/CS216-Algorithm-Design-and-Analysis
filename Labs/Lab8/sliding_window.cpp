#include<bits/stdc++.h>
using namespace std;
const int N=1e6+1;
int a[N],n,k;
struct node
{
    int pos,num;
};
deque<node>dq;
int main(){
    cin>>n>>k;
    for(int i=0;i<n;i++){
        cin>>a[i];
    }

    for(int i=1;i<=n;i++){
        // i永远表示窗口最后一个元素
        // 1 如果元素已经不在窗口内，要从头弹出
        if(!dq.empty()&&dq.front().pos<i-(k-1)){
            dq.pop_front();
        }   
        // 2 如果入队的元素，比队尾的元素要大
        while(!dq.empty()&&a[i]>=dq.back().num){
            dq.pop_back();
        }
        // 3 a[i]需要入队
        dq.push_back({i,a[i]});
        // 4 输出
        if(i>=k){ //保证有了完整的窗口
            cout<<dq.front().num<<" ";
        }
    }
    return 0;
}