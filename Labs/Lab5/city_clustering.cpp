#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
struct edge{
    int v1;
    int v2;
    int distance;
    edge(int v1,int v2,int distance):v1(v1),v2(v2),distance(distance){}
    bool operator<(const edge& e) const{
        return distance<e.distance;
    }
};
vector<int> parent;

int find(int x){
    if(parent[x]==x)return x;
    return parent[x]=find(parent[x]); //在查找根节点的过程中，将 x 的父节点直接指向根节点;后续查找会更快
}
void unite(int x,int y){
    parent[find(x)]=find(y);// 直接将一个连通分量的根节点挂到另一个连通分量的根节点下
}
int main(){
    int n,m,k;
    cin>>n>>m>>k;

    vector<edge> edges;
    for(int i=0;i<m;i++){
        int v1,v2,distance;
        cin>>v1>>v2>>distance;
        edge e=edge(v1,v2,distance);
        edges.push_back(e);
    }

    
    sort(edges.begin(),edges.end());

    parent.resize(n+1);
    for(int i=1;i<=n;i++){
        parent[i]=i;
    }

    int clusters=n;
    vector<int> mst_edges;
    for(auto& e : edges){
        if(find(e.v1)!=find(e.v2)){
            unite(e.v1,e.v2);
            clusters--;
        }
        if(clusters==k-1){
            cout<<e.distance<<endl;
            return 0;
        }
    }
}