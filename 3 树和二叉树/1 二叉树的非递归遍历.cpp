#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;
// 定义二叉树节点的结构体
struct TreeNode 
{
    char value; // 节点的值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    // 构造函数，使用初始化列表初始化成员变量
    TreeNode(char val) : value(val), left(nullptr), right(nullptr) {}
};
// 后序遍历函数，递归实现
void postOrderTraversal(TreeNode* node, vector<char>& result) 
{
    if (node == nullptr)
        return; // 如果节点为空，直接返回
    postOrderTraversal(node->left, result); // 递归遍历左子树
    postOrderTraversal(node->right, result); // 递归遍历右子树
    result.push_back(node->value); // 访问当前节点
}

int main() 
{
    int n;
    cin >> n; // 读取二叉树的节点个数
    stack<TreeNode*> s; // 定义一个栈用于构造二叉树
    TreeNode* root = nullptr; // 根节点初始化为空
    TreeNode* current = nullptr; // 当前节点初始化为空

    // 读取2n行的栈操作
    for (int i = 0; i < 2 * n; ++i) 
    {
        string operation;
        cin >> operation; // 读取操作类型
        if (operation == "push") 
        {
            char value;
            cin >> value; // 读取节点值
            TreeNode* node = new TreeNode(value); // 创建新节点
            if (root == nullptr) 
            {
                root = node; // 如果根节点为空，则将新节点设为根节点
            }
            else 
            {
                if (current->left == nullptr) 
                {
                    current->left = node; // 如果当前节点的左子节点为空，则将新节点设为左子节点
                }
                else
                {
                    current->right = node; // 否则将新节点设为右子节点
                }
            }
            s.push(node); // 将新节点压入栈中
            current = node; // 更新当前节点
        }
        else if (operation == "pop")
        {
            current = s.top(); // 弹出栈顶节点
            s.pop();
        }
    }
    vector<char> postOrder; // 用于存储后序遍历结果
    postOrderTraversal(root, postOrder); // 进行后序遍历
    for (char c : postOrder)
    {
        cout << c; // 输出后序遍历结果
    }
    cout << endl;
    return 0;
}