#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

const int MAXN = 2000; //定义最大节点数
vector<int> adj[MAXN + 1]; //邻接表

//广度优先搜索函数，计算从start节点出发，距离不超过6的节点数
float bfs(int start, int n) {
    vector<int> distance(n + 1, -1); //记录每个节点的距离，初始为-1
    queue<int> q; //队列用于BFS
    q.push(start); //起始节点入队
    distance[start] = 0; //起始节点距离为0
    int count = 0; //计数器

    while (!q.empty()) {
        int node = q.front(); //取队首节点
        q.pop(); //队首节点出队
        if (distance[node] > 6) break; //如果距离超过6，停止搜索
        count++; //计数器加1
        for (int neighbor : adj[node]) { //遍历当前节点的所有邻居
            if (distance[neighbor] == -1) { //如果邻居节点未被访问
                distance[neighbor] = distance[node] + 1; //更新邻居节点的距离
                q.push(neighbor); //邻居节点入队
            }
        }
    }
    return (float)count / n * 100; //计算百分比并返回
}

int main() {
    int n, m;
    cin >> n >> m; //读取节点数和边数
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v; //读取每条边
        adj[u].push_back(v); //构建邻接表
        adj[v].push_back(u); //无向图，双向添加
    }

    for (int i = 1; i <= n; ++i) {
        float percentage = bfs(i, n); //计算每个节点的百分比
        cout << i << ": " << fixed << setprecision(2) << percentage << "%" << endl; //输出结果
    }

    return 0;
}