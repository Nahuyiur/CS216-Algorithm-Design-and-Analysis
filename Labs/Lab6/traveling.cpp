#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>
#include<utility>
using namespace std;
struct node{
    int pre;
    int next;
    int cost;//这个是单独的cost
    int total_cost;
    int num_count;
    int last_city;
};

int main(){
    int n,root;
    cin>>n>>root;

    vector<bool> visited(n+1,false);
    priority_queue<pair<double,int>> max_heap;//int存这个超点的第一个点，double存平均cost
    vector<node>nodes(n+1);
    for(int i=1;i<=n;i++){
        cin>>nodes[i].cost;
        nodes[i].num_count=1;
        nodes[i].total_cost=nodes[i].cost;
        nodes[i].last_city=i;
        if(i!=root)max_heap.push({nodes[i].cost,i});//默认按照第一个属性来排序
    }

    int pre_node,next_node;
    for(int i=1;i<n;i++){
        cin>>pre_node>>next_node;
        nodes[next_node].pre=pre_node;
    }

    while (!max_heap.empty()) {
        int max_node_id = max_heap.top().second;
        max_heap.pop();

        if (visited[max_node_id]) continue;
        visited[max_node_id] = true;

        int pre_node_id = nodes[max_node_id].pre;
        while (visited[pre_node_id] && pre_node_id != root) {
            pre_node_id = nodes[pre_node_id].pre;
        }

        nodes[nodes[pre_node_id].last_city].next = max_node_id;
        nodes[pre_node_id].last_city = nodes[max_node_id].last_city;
        nodes[pre_node_id].num_count += nodes[max_node_id].num_count;
        nodes[pre_node_id].total_cost += nodes[max_node_id].total_cost;

        if (pre_node_id != root) {
            max_heap.push({(double)nodes[pre_node_id].total_cost / nodes[pre_node_id].num_count, pre_node_id});
        }
    }

    long long ans = 0;
    int pos = root, time = 0;
    while(pos){
        time++;
        ans += nodes[pos].cost * time;
        pos = nodes[pos].next;
    }
    cout << ans << endl;
}