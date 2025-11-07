#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 定义树节点结构
struct TreeNode {
    char data; // 节点数据
    int left, right; // 左右孩子的索引
};

// 定义二叉树节点结构
struct Node {
    char data; // 节点数据
    Node* left; // 左孩子指针
    Node* right; // 右孩子指针
    Node(char d) : data(d), left(nullptr), right(nullptr) {} // 构造函数
};

// 根据节点数组和索引构建二叉树
Node* buildTree(const vector<TreeNode>& nodes, int index) {
    if (index == -1) return nullptr; // 如果索引为-1，返回空指针
    Node* root = new Node(nodes[index].data); // 创建新节点
    root->left = buildTree(nodes, nodes[index].left); // 递归构建左子树
    root->right = buildTree(nodes, nodes[index].right); // 递归构建右子树
    return root; // 返回根节点
}

// 判断两棵树是否同构
bool isIsomorphic(Node* T1, Node* T2)
{
    if (T1 == nullptr && T2 == nullptr) return true; // 如果两棵树都为空，返回true
    if (T1 == nullptr || T2 == nullptr) return false; // 如果其中一棵树为空，返回false
    if (T1->data != T2->data) return false; // 如果根节点数据不同，返回false
    // 递归判断左右子树是否同构
    return (isIsomorphic(T1->left, T2->left) && isIsomorphic(T1->right, T2->right)) ||
        (isIsomorphic(T1->left, T2->right) && isIsomorphic(T1->right, T2->left));
}

// 计算二叉树的深度
int getDepth(Node* root) {
    if (root == nullptr) return 0; // 如果节点为空，深度为0
    // 递归计算左右子树的深度，返回较大值加1
    return max(getDepth(root->left), getDepth(root->right)) + 1;
}

// 找到树的根节点
int findRoot(const vector<TreeNode>& nodes) {
    int n = nodes.size();
    vector<bool> isChild(n, false); // 标记节点是否为孩子节点
    for (const auto& node : nodes) {
        if (node.left != -1) isChild[node.left] = true; // 标记左孩子
        if (node.right != -1) isChild[node.right] = true; // 标记右孩子
    }
    for (int i = 0; i < n; ++i) {
        if (!isChild[i]) return i; // 找到没有被标记为孩子的节点，即为根节点
    }
    return -1; // 如果没有找到根节点，返回-1
}

int main() {
    int N1, N2;
    cin >> N1; // 输入第一棵树的节点数
    vector<TreeNode> nodes1(N1);
    for (int i = 0; i < N1; ++i) {
        char data;
        string left, right;
        cin >> data >> left >> right; // 输入节点数据和左右孩子
        nodes1[i] = { data, left == "-" ? -1 : stoi(left), right == "-" ? -1 : stoi(right) };
    }
    cin >> N2; // 输入第二棵树的节点数
    vector<TreeNode> nodes2(N2);
    for (int i = 0; i < N2; ++i) {
        char data;
        string left, right;
        cin >> data >> left >> right; // 输入节点数据和左右孩子
        nodes2[i] = { data, left == "-" ? -1 : stoi(left), right == "-" ? -1 : stoi(right) };
    }

    int root1 = findRoot(nodes1); // 找到第一棵树的根节点
    int root2 = findRoot(nodes2); // 找到第二棵树的根节点
    Node* T1 = buildTree(nodes1, root1); // 构建第一棵树
    Node* T2 = buildTree(nodes2, root2); // 构建第二棵树

    if (isIsomorphic(T1, T2)) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }
    cout << getDepth(T1) << endl << getDepth(T2) << endl;
    return 0;
}