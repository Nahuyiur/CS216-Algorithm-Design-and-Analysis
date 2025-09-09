#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAX_NODE = 2000;
const int MAX_EDGE = 500000;
const int INF = 0x3f3f3f3f;

struct FlowEdge {
    int to, next, capacity;
};

FlowEdge edges[MAX_EDGE];
int head[MAX_NODE], edgeIndex = 0;
int level[MAX_NODE], current[MAX_NODE];
int lowerBound[1505], upperBound[1505], netDemand[MAX_NODE];
int totalFlowRequired = 0, currentMaxFlow = 0;

int source, sink;

void insertEdge(int u, int v, int cap) {
    edges[edgeIndex] = {v, head[u], cap};
    head[u] = edgeIndex++;
    edges[edgeIndex] = {u, head[v], 0};
    head[v] = edgeIndex++;
}

bool buildLevelGraph() {
    memset(level, 0x3f, sizeof(level));
    queue<int> q;
    q.push(source);
    level[source] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = head[u]; i != -1; i = edges[i].next) {
            int v = edges[i].to;
            if (edges[i].capacity > 0 && level[v] == INF) {
                level[v] = level[u] + 1;
                if (v == sink) return true;
                q.push(v);
            }
        }
    }
    return level[sink] != INF;
}

int sendFlow(int u, int flow) {
    if (u == sink) {
        currentMaxFlow += flow;
        return flow;
    }
    for (int &i = current[u]; i != -1 && flow > 0; i = edges[i].next) {
        int v = edges[i].to;
        if (edges[i].capacity > 0 && level[v] == level[u] + 1) {
            int pushed = sendFlow(v, min(edges[i].capacity, flow));
            if (pushed > 0) {
                edges[i].capacity -= pushed;
                edges[i ^ 1].capacity += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

void runDinic() {
    while (buildLevelGraph()) {
        memcpy(current, head, sizeof(head));
        while (sendFlow(source, INF));
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(head, -1, sizeof(head));

    int days, workers;
    cin >> days >> workers;

    source = 0;
    sink = days + workers + 3;
    int superSource = 1;
    int superSink = days + workers + 2;
    int offset = 2;

    for (int i = 0; i < workers; ++i) {
        cin >> lowerBound[i];
    }

    for (int d = 0; d < days; ++d) {
        int assignedCount, dailyCap;
        cin >> assignedCount >> dailyCap;

        int taskNode = offset + workers + d;
        for (int j = 0; j < assignedCount; ++j) {
            int workerId, lb, ub;
            cin >> workerId >> lb >> ub;
            int workerNode = offset + workerId;
            insertEdge(workerNode, taskNode, ub - lb);
            netDemand[taskNode] += lb;
            netDemand[workerNode] -= lb;
            upperBound[workerId] += ub;
        }
        insertEdge(taskNode, superSink, dailyCap);
    }

    for (int i = 0; i < workers; ++i) {
        if (lowerBound[i] > upperBound[i]) {
            cout << -1 << endl;
            return 0;
        }
        int workerNode = offset + i;
        insertEdge(superSource, workerNode, upperBound[i] - lowerBound[i]);
        netDemand[superSource] -= lowerBound[i];
        netDemand[workerNode] += lowerBound[i];
    }

    for (int i = 1; i < sink; ++i) {
        if (netDemand[i] > 0) {
            insertEdge(source, i, netDemand[i]);
            totalFlowRequired += netDemand[i];
        } else if (netDemand[i] < 0) {
            insertEdge(i, sink, -netDemand[i]);
        }
    }

    insertEdge(superSink, superSource, INF);

    runDinic();

    if (currentMaxFlow >= totalFlowRequired) {
        int loopEdgeCap = edges[edgeIndex - 1].capacity;
        currentMaxFlow = 0;

        source = superSource;
        sink = superSink;

        edges[edgeIndex - 1].capacity = 0;
        edges[edgeIndex - 2].capacity = 0;

        runDinic();

        cout << currentMaxFlow + loopEdgeCap << '\n';
    } else {
        cout << -1 << '\n';
    }

    return 0;
}
