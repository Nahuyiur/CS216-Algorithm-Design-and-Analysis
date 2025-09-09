#include<iostream>
using namespace std;
bool judgeWinner(int n){
    if(n%6==0){
        return false;
    }
    return true;
}
void calWinner(int n){
    if(judgeWinner(n)){
        cout<<"Alice"<<endl;
    }else{
        cout<<"Bob"<<endl;
    }
}
int main(){
    int n;
    cin>>n;

    int a[n];
    for (size_t i = 0; i < n; i++)
    {
        cin>>a[i];
    }
    
    for (size_t i = 0; i < n; i++)
    {
        calWinner(a[i]);
    }
    
    return 0;
}