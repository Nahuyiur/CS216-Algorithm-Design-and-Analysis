#include<iostream>
using namespace std;
struct edge
{
    int from,to,cost;
    edge(int f,int t,int c):from(f),to(t),cost(c){}
};

vector<edge>edges;
vector<vector<pair<int,int>>> graph;//邻接表表示图
int n,m;
vector<int> parent;//记录谁给谁发信息

//选择每个点的最小入边
vector<edge> select_min_edge(int root){
    vector<edge> min_edges(n+1,edge(-1,-1,INT_MAX));
    for(auto& e:edges){
        if(e.to==root) continue;//入边指向根节点需要跳过
        if(e.cost<min_edges[e.to].cost){
            min_edges[e.to]=e;
        }else if(e.cost==min_edges[e.to].cost&&e.from<min_edges[e.to].from){// 边权相等时，选择编号较小的点
            min_edges[e.to]=e;
        }
    }
    return min_edges;
}

//用dfs检测环
bool dfs_cycle()
int main(){
    cin>>n>>m;

    int total_w=0;
    for(int i=0;i<m;i++){
        int s,t,c;
        cin>>s>>t>>c;
        total_w+=c;
    }
    //设置虚点是n


    return 0;
}