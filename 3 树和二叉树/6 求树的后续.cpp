#include <iostream>
#include <string>

using namespace std;

// 递归函数，用于根据前序和中序遍历构建后序遍历
bool BuildPostorder(const string& preorder, const string& inorder, string& postorder) {
    if (preorder.empty() || inorder.empty()) {
        return true;
    }

    // 前序遍历的第一个节点是根节点
    char rootValue = preorder[0];
    size_t rootIndex = inorder.find(rootValue);

    // 如果在中序遍历中找不到根节点，说明输入不合法
    if (rootIndex == string::npos) {
        return false;
    }

    // 递归处理左子树
    if (!BuildPostorder(preorder.substr(1, rootIndex), inorder.substr(0, rootIndex), postorder)) {
        return false;
    }
    // 递归处理右子树
    if (!BuildPostorder(preorder.substr(rootIndex + 1), inorder.substr(rootIndex + 1), postorder)) {
        return false;
    }
    // 将根节点添加到后序遍历结果中
    postorder += rootValue;

    return true;
}

int main() {
    string input;
    while (getline(cin, input)) {
        size_t spaceIndex = input.find(' ');
        if (spaceIndex == string::npos) {
            cout << "Error" << endl;
            continue;
        }

        string preorder = input.substr(0, spaceIndex);
        string inorder = input.substr(spaceIndex + 1);
        string postorder;

        if (BuildPostorder(preorder, inorder, postorder)) {
            cout << postorder << endl;
        }
        else {
            cout << "Error" << endl;
        }
    }

    return 0;
}

