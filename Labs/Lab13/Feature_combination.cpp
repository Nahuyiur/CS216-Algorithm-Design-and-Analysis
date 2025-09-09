#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>

using namespace std;

// 常量定义
const int MAX_CREATURES = 6E5;  // 最大生物数
const int BASE_CREATURES = 9E4;  // 基础生物数
const int MAX_FEATURES = 100;  // 每个生物最多的特征数

// 数据结构定义
vector<int> featureWeights(MAX_CREATURES + 1);  // 每个特征的权重
unsigned long long featureCount[MAX_CREATURES + 1];  // 每个新生物的特征计数
int parentCreature1[MAX_CREATURES + 1], parentCreature2[MAX_CREATURES + 1];  // 每个新生物父生物的索引
int creatureFeatures[MAX_CREATURES + 1][MAX_FEATURES];  // 存储生物的特征
int currentFeatureCount[MAX_CREATURES + 1];  // 当前每个生物的特征数量

// 合并两个生物的特征
void mergeFeatures(int source1, int source2, int destination) {
    int idx1 = 0, idx2 = 0, idxDest = 0;

    // 合并特征，按权重从小到大
    while (idx1 < currentFeatureCount[source1] && idx2 < currentFeatureCount[source2] && idxDest < MAX_FEATURES) {
        if (creatureFeatures[source1][idx1] == creatureFeatures[source2][idx2]) {
            creatureFeatures[destination][idxDest] = creatureFeatures[source1][idx1];
            ++idx1;
            ++idx2;
            ++idxDest;
        } else if (featureWeights[creatureFeatures[source1][idx1]] < featureWeights[creatureFeatures[source2][idx2]]) {
            creatureFeatures[destination][idxDest] = creatureFeatures[source1][idx1];
            ++idx1;
            ++idxDest;
        } else {
            creatureFeatures[destination][idxDest] = creatureFeatures[source2][idx2];
            ++idx2;
            ++idxDest;
        }
    }

    // 将剩余特征添加到目标生物中
    while (idx1 < currentFeatureCount[source1] && idxDest < MAX_FEATURES) {
        creatureFeatures[destination][idxDest] = creatureFeatures[source1][idx1];
        ++idx1;
        ++idxDest;
    }
    while (idx2 < currentFeatureCount[source2] && idxDest < MAX_FEATURES) {
        creatureFeatures[destination][idxDest] = creatureFeatures[source2][idx2];
        ++idx2;
        ++idxDest;
    }

    // 更新目标生物的特征数量
    currentFeatureCount[destination] = idxDest;
}

int main() {
    int totalCreatures, baseCreatures;
    cin >> totalCreatures >> baseCreatures;

    // 初始化权重：每个特征的权重为其编号
    for (int i = 0; i <= MAX_CREATURES; ++i) {
        featureWeights[i] = i;
    }

    random_device rd;
    mt19937 rng(rd());

    // 读取生物组合信息
    for (int i = baseCreatures + 1; i <= totalCreatures; ++i) {
        cin >> parentCreature1[i] >> parentCreature2[i];
    }

    // 进行 3 次打乱，确保随机性
    for (int rand_iter = 0; rand_iter < 3; ++rand_iter) {
        // 打乱特征的权重
        shuffle(featureWeights.begin() + 1, featureWeights.end(), rng);

        // 重置每个生物的特征数量
        memset(currentFeatureCount, 0, sizeof(currentFeatureCount));

        // 初始化基础生物
        for (int i = 1; i <= baseCreatures; ++i) {
            creatureFeatures[i][0] = i;
            currentFeatureCount[i] = 1;
        }

        // 合并生物并计算特征数量
        for (int i = baseCreatures + 1; i <= totalCreatures; ++i) {
            mergeFeatures(parentCreature1[i], parentCreature2[i], i);

            // 计算当前新生物的特征数量
            if (currentFeatureCount[i] == MAX_FEATURES) {
                int lastFeature = creatureFeatures[i][currentFeatureCount[i] - 1];
                featureCount[i - baseCreatures] += (unsigned long long)MAX_CREATURES * MAX_FEATURES / (featureWeights[lastFeature] + 1);
            } else {
                featureCount[i - baseCreatures] += currentFeatureCount[i];
            }
        }
    }

    // 输出每个新生物的特征数量
    for (int i = 1; i <= totalCreatures - baseCreatures; ++i) {
        cout << featureCount[i] / 3 << endl;
    }

    return 0;
}
