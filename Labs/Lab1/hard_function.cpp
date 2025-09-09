#include<iostream>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;
int main()
{
    int n,k;
    cin>>n>>k;

    if(n==1&&k==1)
    {
        cout<<"1"<<endl;
    }else{
        int ans=2*k;
        if(ans>2*(n-k+1))ans=2*(n-k+1);
        cout<<ans<<endl;
    }
    return 0;
}