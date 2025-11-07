#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// 拓扑排序函数，返回拓扑排序后的节点顺序
vector<int> topologicalSort(int k, const vector<pair<int, int>>& conditions) {
    vector<int> inDegree(k + 1, 0); // 入度数组
    unordered_map<int, vector<int>> graph; // 邻接表表示的图
    for (const auto& condition : conditions) {
        graph[condition.first].push_back(condition.second);
        inDegree[condition.second]++;
    }

    queue<int> q;
    for (int i = 1; i <= k; ++i) {
        if (inDegree[i] == 0) {
            q.push(i); // 入度为0的节点入队
        }
    }

    vector<int> order;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);
        for (int neighbor : graph[node]) {
            if (--inDegree[neighbor] == 0) {
                q.push(neighbor); // 入度为0的邻居节点入队
            }
        }
    }

    return order.size() == k ? order : vector<int>(); // 如果排序结果包含所有节点则返回排序结果，否则返回空
}

// 构建矩阵函数
vector<vector<int>> buildMatrix(int k, const vector<pair<int, int>>& rowConditions, const vector<pair<int, int>>& colConditions) {
    vector<int> rowOrder = topologicalSort(k, rowConditions); // 行的拓扑排序
    vector<int> colOrder = topologicalSort(k, colConditions); // 列的拓扑排序

    if (rowOrder.empty() || colOrder.empty()) {
        return {}; // 如果任一排序结果为空，返回空矩阵
    }

    unordered_map<int, int> rowIndex, colIndex;
    for (int i = 0; i < k; ++i) {
        rowIndex[rowOrder[i]] = i; // 记录每个元素在行中的位置
        colIndex[colOrder[i]] = i; // 记录每个元素在列中的位置
    }

    vector<vector<int>> matrix(k, vector<int>(k, 0)); // 初始化k x k的矩阵
    for (int i = 1; i <= k; ++i) {
        matrix[rowIndex[i]][colIndex[i]] = i; // 根据行列位置填充矩阵
    }

    return matrix;
}

int main() {
    int k, n, m;
    cin >> k >> n >> m; // 输入k, n, m

    vector<pair<int, int>> rowConditions(n), colConditions(m);
    for (int i = 0; i < n; ++i) {
        cin >> rowConditions[i].first >> rowConditions[i].second; // 输入行条件
    }
    for (int i = 0; i < m; ++i) {
        cin >> colConditions[i].first >> colConditions[i].second; // 输入列条件
    }

    vector<vector<int>> result = buildMatrix(k, rowConditions, colConditions); // 构建矩阵
    if (result.empty()) {
        cout << -1 << endl; // 如果结果为空，输出-1
    }
    else {
        for (const auto& row : result) {
            for (int num : row) {
                cout << num << " "; // 输出矩阵
            }
            cout << endl;
        }
    }

    return 0;
}
