#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 边结构体，包含两个端点和边的权重
struct Edge {
    int u, v, weight;
    // 重载小于运算符，用于边的排序
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// 并查集查找函数，带路径压缩
int find(vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

// 并查集合并函数，带秩优化
void unite(vector<int>& parent, vector<int>& rank, int u, int v) {
    int rootU = find(parent, u);
    int rootV = find(parent, v);
    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        }
        else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        }
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

int main() {
    int n;
    cin >> n; // 读取村庄数目
    vector<Edge> edges;
    vector<vector<int>> dist(n, vector<int>(n));

    // 读取距离矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> dist[i][j];
            if (i < j) {
                edges.push_back({ i, j, dist[i][j] });
            }
        }
    }

    int m;
    cin >> m; // 读取已经修建的道路数目
    vector<int> parent(n), rank(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    // 处理已经修建的道路
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        unite(parent, rank, a - 1, b - 1);
    }

    // 对所有边按权重排序
    sort(edges.begin(), edges.end());

    int mst_weight = 0;
    // Kruskal算法构建最小生成树
    for (const auto& edge : edges) {
        if (find(parent, edge.u) != find(parent, edge.v)) {
            unite(parent, rank, edge.u, edge.v);
            mst_weight += edge.weight;
        }
    }

    // 输出最小生成树的权重
    cout << mst_weight << endl;
    return 0;
}
