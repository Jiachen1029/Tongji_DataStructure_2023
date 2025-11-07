#include <iostream>
#include <cmath>
#include <stack>
using namespace std;

int main() {
    int cnt = 0; // 记录树的数量
    char t[1000000]; // 存储输入字符串
    while (cin >> t) {
        if (t[0] == '#')
            break; // 遇到 '#' 结束输入
        int nh = 0, h = 0, nh2 = 0, h2 = 0; // 初始化高度变量
        stack<int> now; // 栈用于记录二叉树的节点高度
        now.push(0); // 初始节点高度为 0
        for (int i = 0; t[i] != '\0'; i++) {
            if (t[i] == 'd') {
                nh++; // 原始树当前高度增加
                nh2++; // 二叉树当前高度增加
                now.push(nh2); // 将当前节点高度压入栈中
            }
            else {
                nh--; // 原始树当前高度减少
                nh2 = now.top(); // 获取栈顶元素，表示当前节点的父节点高度
                now.pop(); // 当前节点出栈
            }
            h2 = max(h2, nh2); // 更新二叉树的最大高度
            h = max(h, nh); // 更新原始树的最大高度
        }
        cout << "Tree " << ++cnt << ": " << h << " => " << h2 << endl; // 输出结果
    }
    return 0;
}