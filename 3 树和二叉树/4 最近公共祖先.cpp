//¶à²æÊ÷£¡£¡£¡
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Tree {
public:
    unordered_map<int, int> parent;
    unordered_map<int, unordered_set<int>> children;

    void addEdge(int parent, int child) {
        this->parent[child] = parent;
        this->children[parent].insert(child);
    }

    int findLCA(int x, int y) {
        unordered_set<int> ancestors;
        while (x != 0) {
            ancestors.insert(x);
            x = parent[x];
        }
        while (y != 0) {
            if (ancestors.find(y) != ancestors.end()) {
                return y;
            }
            y = parent[y];
        }
        return -1;
    }
};

int main() {
    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        Tree tree;
        for (int i = 0; i < N - 1; ++i) {
            int a, b;
            cin >> a >> b;
            tree.addEdge(a, b);
        }
        for (int i = 0; i < M; ++i) {
            int x, y;
            cin >> x >> y;
            cout << tree.findLCA(x, y) << endl;
        }
    }
    return 0;
}
