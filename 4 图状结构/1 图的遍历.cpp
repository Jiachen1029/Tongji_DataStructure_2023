#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#define MAX_NUM 2000

using namespace std;

typedef struct MGraph {
    int vexs[MAX_NUM] = { 0 }; // ½Úµã
    vector<vector<int>> arcs;  // ÁÚ½Ó¾ØÕó
    int vexnum, arcnum;
    MGraph() : arcs(MAX_NUM, vector<int>(MAX_NUM, 0)) {
        
    }
} MGraph;

class Picture {
public:
    enum SEARCH_TYPE { dfs, bfs };
    void CreateMGraph() {
        cin >> G.vexnum >> G.arcnum;
        for (int i = 0; i < G.arcnum; i++) {
            int y, x;
            cin >> y >> x;
            G.arcs[y][x] = 1;
            G.arcs[x][y] = 1;
        }
    }
    void Search(SEARCH_TYPE type) {
        for (int i = 0; i < MAX_NUM; i++)
            visited[i] = 0;
        for (int i = 0; i < G.vexnum; i++) {
            if (!visited[i]) {
                if (type == SEARCH_TYPE::dfs)
                    SearchByDfs(i);
                else
                    SearchByBfs(i);
                cout << '{';
                while (!DFS_BFS.empty()) {
                    cout << DFS_BFS.front();
                    DFS_BFS.pop_front();
                    if (!DFS_BFS.empty())
                        cout << ' ';
                }
                cout << '}';
            }
        }
        cout << endl;
    }
private:
    MGraph G;
    bool visited[MAX_NUM];
    deque<int> DFS_BFS;
    void SearchByBfs(int start) {
        queue<int> Q;
        Q.push(start);
        visited[start] = 1;
        while (!Q.empty()) {
            int now = Q.front();
            Q.pop();
            DFS_BFS.push_back(now);
            for (int i = 0; i < G.vexnum; i++) {
                if (G.arcs[now][i] && !visited[i]) {
                    Q.push(i);
                    visited[i] = 1;
                }
            }
        }
    }
    void SearchByDfs(int now) {
        DFS_BFS.push_back(now);
        visited[now] = 1;
        for (int i = 0; i < G.vexnum; i++) {
            if (G.arcs[now][i] && !visited[i]) {
                SearchByDfs(i);
            }
        }
    }
};

int main() {
    Picture pic;
    pic.CreateMGraph();
    pic.Search(Picture::SEARCH_TYPE::dfs);
    pic.Search(Picture::SEARCH_TYPE::bfs);
    return 0;
}