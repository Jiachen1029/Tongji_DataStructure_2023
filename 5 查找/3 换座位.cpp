/**
 * @file    template.cpp
 * @name    p138模板程序
 * @date    2022-11-20
*/

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
using namespace std;


/********************************/
/*     以下是你需要提交的代码     */
/********************************/
class Solution {
public:
    int solve(std::vector<std::vector<std::string>>& old_chart, std::vector<std::vector<std::string>>& new_chart) {
        int n = old_chart.size();
        int m = old_chart[0].size();
        std::map<std::string, std::pair<int, int>> old_positions;
        std::map<std::string, std::pair<int, int>> new_positions;

        // 记录每个学生在old_chart中的位置
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                old_positions[old_chart[i][j]] = { i, j };
            }
        }

        // 记录每个学生在new_chart中的位置
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                new_positions[new_chart[i][j]] = { i, j };
            }
        }

        std::vector<bool> visited(n * m, false);
        int swaps = 0;

        // 遍历每个学生，找到所有的环
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (!visited[i * m + j]) {
                    int cycle_length = 0;
                    int x = i, y = j;

                    // 找到一个环
                    while (!visited[x * m + y]) {
                        visited[x * m + y] = true;
                        auto new_pos = new_positions[old_chart[x][y]];
                        x = new_pos.first;
                        y = new_pos.second;
                        ++cycle_length;
                    }

                    // 环的长度减一就是所需的交换次数
                    if (cycle_length > 1) {
                        swaps += cycle_length - 1;
                    }
                }
            }
        }

        return swaps;
    }
};

/********************************/
/*     以上是你需要提交的代码     */
/********************************/

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::string>> old_chart(n, std::vector<std::string>(m));
    std::vector<std::vector<std::string>> new_chart(n, std::vector<std::string>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> old_chart[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> new_chart[i][j];
        }
    }

    Solution s;
    std::cout << s.solve(old_chart, new_chart) << std::endl;
    return true;
}
