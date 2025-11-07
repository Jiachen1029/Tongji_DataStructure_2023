#include <iostream>

using namespace std;

// 定义二叉排序树的节点结构
struct Node {
    int key;       // 节点的键值
    int count;     // 相同键值的数量
    Node* left;    // 左子节点指针
    Node* right;   // 右子节点指针
    Node(int k) : key(k), count(1), left(nullptr), right(nullptr) {}
};

// 插入节点
Node* insert(Node* root, int key) {
    if (root == nullptr) {
        // 如果根节点为空，创建新节点
        return new Node(key);
    }
    if (key == root->key) {
        // 键值相同，计数加一
        root->count++;
    }
    else if (key < root->key) {
        // 键值小于当前节点，递归插入左子树
        root->left = insert(root->left, key);
    }
    else {
        // 键值大于当前节点，递归插入右子树
        root->right = insert(root->right, key);
    }
    return root;
}

// 查找最小值节点
Node* findMin(Node* root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

// 删除节点
Node* remove(Node* root, int key, bool& found) {
    if (root == nullptr) {
        // 未找到要删除的节点
        found = false;
        return nullptr;
    }
    if (key < root->key) {
        // 在左子树中删除
        root->left = remove(root->left, key, found);
    }
    else if (key > root->key) {
        // 在右子树中删除
        root->right = remove(root->right, key, found);
    }
    else {
        // 找到要删除的节点
        found = true;
        if (root->count > 1) {
            // 如果节点有重复，计数减一
            root->count--;
        }
        else {
            if (root->left != nullptr && root->right != nullptr) {
                // 节点有两个子节点，找到右子树的最小值替代
                Node* minNode = findMin(root->right);
                root->key = minNode->key;
                root->count = minNode->count;
                bool temp;
                // 删除右子树中的最小值节点
                root->right = remove(root->right, minNode->key, temp);
            }
            else {
                // 节点有一个或零个子节点
                Node* temp = root;
                if (root->left != nullptr) {
                    // 用左子节点替代
                    root = root->left;
                }
                else {
                    // 用右子节点替代
                    root = root->right;
                }
                delete temp;  // 释放内存
            }
        }
    }
    return root;
}

// 查询键值的数量
int queryCount(Node* root, int key) {
    if (root == nullptr) {
        // 未找到，返回0
        return 0;
    }
    if (key == root->key) {
        // 找到，返回计数
        return root->count;
    }
    else if (key < root->key) {
        // 在左子树中查询
        return queryCount(root->left, key);
    }
    else {
        // 在右子树中查询
        return queryCount(root->right, key);
    }
}

// 查找最小值
int findMinValue(Node* root) {
    Node* minNode = findMin(root);
    return minNode->key;
}

// 查找前驱节点
int findPredecessor(Node* root, int key, int pred = -1) {
    if (root == nullptr) {
        // 未找到前驱，返回当前前驱值
        return pred;
    }
    if (key == root->key) {
        if (root->left != nullptr) {
            // 前驱为左子树的最右节点
            Node* temp = root->left;
            while (temp->right != nullptr) {
                temp = temp->right;
            }
            return temp->key;
        }
        else {
            // 无左子树，前驱为已记录的 pred
            return pred;
        }
    }
    else if (key < root->key) {
        // 在左子树中查找
        return findPredecessor(root->left, key, pred);
    }
    else {
        // 更新前驱为当前节点，继续在右子树中查找
        pred = root->key;
        return findPredecessor(root->right, key, pred);
    }
}

int main() {
    int n;
    cin >> n;
    Node* root = nullptr;
    while (n--) {
        int op, x;
        cin >> op;
        if (op == 1) {
            // 插入操作
            cin >> x;
            root = insert(root, x);
        }
        else if (op == 2) {
            // 删除操作
            cin >> x;
            bool found = false;
            root = remove(root, x, found);
            if (!found) {
                cout << "None" << endl;
            }
        }
        else if (op == 3) {
            // 查询数量操作
            cin >> x;
            int cnt = queryCount(root, x);
            cout << cnt << endl;
        }
        else if (op == 4) {
            // 查询最小值操作
            int minValue = findMinValue(root);
            cout << minValue << endl;
        }
        else if (op == 5) {
            // 查询前驱操作
            cin >> x;
            int pred = findPredecessor(root, x);
            if (pred == -1) {
                cout << "None" << endl;
            }
            else {
                cout << pred << endl;
            }
        }
    }
    return 0;
}
