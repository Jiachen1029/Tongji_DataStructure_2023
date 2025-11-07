/**
 * @name    template.cpp
 * @brief   p142模板程序
 * @date    2022-12-02
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
class FreqStack {
private:
    map<int, int> freq; // 记录每个值的频率
    map<int, stack<int>> group; // 记录每个频率对应的值的栈
    int maxFreq; // 当前的最大频率

public:
    FreqStack() : maxFreq(0) {}

    void push(int val) {
        int f = ++freq[val];
        if (f > maxFreq) {
            maxFreq = f;
        }
        group[f].push(val);
    }

    int pop() {
        int val = group[maxFreq].top();
        group[maxFreq].pop();
        if (group[maxFreq].empty()) {
            maxFreq--;
        }
        freq[val]--;
        return val;
    }
};
/********************************/
/*     以上是你需要提交的代码     */
/********************************/

int main() {
    int n;
    std::cin >> n;
    FreqStack fs;
    while (n --) {
        std::string order;
        std::cin >> order;
        if (order == "push") {
            int val;
            std::cin >> val;
            fs.push(val);
        }
        else if (order == "pop") {
            std::cout << fs.pop() << std::endl;
        }
    }
    return 0;
}
