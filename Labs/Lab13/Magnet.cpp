#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>

using namespace std;

const int NODES_LIMIT = 60;
const int SHUFFLE_LIMIT = 1000;

vector<int> nodeOrder;
vector<int> cliqueCandidate;
vector<unordered_set<int>> adjacencyMatrix(NODES_LIMIT, unordered_set<int>());

int main() {
    int nodeCount, edgeCount;
    cin >> nodeCount >> edgeCount;

    // 填充节点列表
    for (int i = 1; i <= nodeCount; ++i) {
        nodeOrder.push_back(i);
    }

    // 填充图的边
    for (int i = 0; i < edgeCount; ++i) {
        int u, v;
        cin >> u >> v;
        adjacencyMatrix[u].insert(v);
        adjacencyMatrix[v].insert(u);
    }

    random_device randomGen;
    mt19937 rng(randomGen());
    int maxCliqueSize = 0;

    // 执行多次打乱并检查最大团
    for (int i = 0; i < SHUFFLE_LIMIT; ++i) {
        shuffle(nodeOrder.begin(), nodeOrder.end(), rng);
        cliqueCandidate.clear();  // 清空当前 clique

        // 检查是否可以构成 clique
        for (int j = 0; j < nodeCount; ++j) {
            bool canBeAddedToClique = true;

            for (int k = 0; k < cliqueCandidate.size(); ++k) {
                if (adjacencyMatrix[nodeOrder[j]].find(cliqueCandidate[k]) == adjacencyMatrix[nodeOrder[j]].end()) {
                    canBeAddedToClique = false;
                    break;
                }
            }

            if (canBeAddedToClique) {
                cliqueCandidate.push_back(nodeOrder[j]);
            }
        }

        maxCliqueSize = max(maxCliqueSize, (int)cliqueCandidate.size());
    }

    cout << maxCliqueSize << endl;
    return 0;
}
