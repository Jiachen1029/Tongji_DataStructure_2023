#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    // 读取输入
    int n, start;
    cin >> n >> start;

    // 构建二叉树的邻接表
    vector<vector<int>> tree(n);
    for (int i = 0; i < n; ++i) {
        int left, right;
        cin >> left >> right;
        if (left != -1) {
            tree[i].push_back(left);
            tree[left].push_back(i);
        }
        if (right != -1) {
            tree[i].push_back(right);
            tree[right].push_back(i);
        }
    }
    // BFS算法
    queue<int> q;
    vector<bool> visited(n, false);
    vector<int> time(n, 0);

    q.push(start);
    visited[start] = true;
    time[start] = 0;

    int maxTime = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : tree[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                time[neighbor] = time[node] + 1;
                maxTime = max(maxTime, time[neighbor]);
                q.push(neighbor);
            }
        }
    }

    // 输出结果
    cout << maxTime << endl;
    return 0;
}