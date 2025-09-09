#include<iostream>
#include<vector>
using namespace std;
int main(){
    int n,W;
    cin>>n>>W;

    vector<pair<int,int>> objects;//c and v

    int c,v,k;
    for(int i=0;i<n;i++){
        cin>>c>>v>>k;
        for(int t=1,reminder=k;reminder>0;t*=2){
            if(reminder>=t){
                objects.push_back({c*t,v*t});
                reminder-=t;
            }else{
                objects.push_back({c*reminder,v*reminder});
                break;
            }
        }
    }

    vector<int> M(W+1,0);
    for(int i=0;i<objects.size();i++){
        for(int w=W;w>=1;w--){
            if(objects[i].first>w){
                continue;
            }else{
                M[w]=max(M[w],M[w-objects[i].first]+objects[i].second);
            }
        }
    }

    cout<<M[W]<<endl;
    return 0;
}