#include <iostream>
#include <queue>
#include <deque>
#include <string>

using namespace std;

struct MaxQueue 
{
    queue<int> q;       // 用于存储队列元素的普通队列
    deque<int> maxDeque;    // 用于存储当前最大值的双端队列
    int capacity;           // 队列的容量

    // 构造函数，初始化队列容量
    MaxQueue(int n) : capacity(n) {}

    // 入队操作
    void Enqueue(int v)
    {
        if (q.size() >= capacity)
        {
            cout << "Queue is Full" << endl;
            return;
        }
        q.push(v);
        // 保持 maxDeque 中的元素从大到小排列
        while (!maxDeque.empty() && maxDeque.back() < v) 
        {
            maxDeque.pop_back();
        }
        maxDeque.push_back(v);
    }
    // 出队操作
    void Dequeue() 
    {
        if (q.empty())
        {
            cout << "Queue is Empty" << endl;
            return;
        }
        int removedValue = q.front();
        q.pop();
        cout << removedValue << endl;
        // 如果出队的元素是当前最大值，则从 maxDeque 中移除
        if (removedValue == maxDeque.front()) 
        {
            maxDeque.pop_front();
        }
    }
    // 获取队列中的最大值
    void GetMax() 
    {
        if (q.empty()) 
        {
            cout << "Queue is Empty" << endl;
            return;
        }
        cout << maxDeque.front() << endl;
    }
    // 打印队列中的所有元素
    void PrintQueue() 
    {
        queue<int> tempQueue = q; // 使用 q 以避免名称冲突
        while (!tempQueue.empty())
        {
            cout << tempQueue.front() << " ";
            tempQueue.pop();
        }
        cout << endl;
    }
};

int main()
{
    int n;
    cin >> n;
    MaxQueue maxQueue(n);  // 创建 MaxQueue 结构体实例

    string input;
    while (cin >> input && input != "quit") 
    {
        if (input == "enqueue") 
        {
            int value;
            cin >> value;
            maxQueue.Enqueue(value);
        }
        else if (input == "dequeue") 
        {
            maxQueue.Dequeue();
        }
        else if (input == "max")
        {
            maxQueue.GetMax();
        }
    }
    maxQueue.PrintQueue();
    return 0;
}