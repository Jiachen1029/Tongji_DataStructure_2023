#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
using namespace std;
#define INT_MAX 2147483647

class Graph {
private:
    struct Subject {
        int time_consuming;     // 累计所需时间
        int last_subjects, next_subjects;   // 前驱科目数量和后继科目数量
        // 科目开始的最早和最晚时间
        int min_start_time; // ve
        int max_start_time; // vl
        std::vector<int> next;   // 后继科目列表
        std::vector<int> last;   // 前驱科目列表
        bool important;
        Subject() : last_subjects(0), next_subjects(0), min_start_time(0), max_start_time(INT_MAX), important(0) {}
    };

    int size;
    std::vector<Subject> subjects;

public:
    // 构造函数，生成具有指定参数的随机图
    void f_in() {
        // 科目数量
        cin >> size;
        for (int i = 0; i < size; i++) {
            Subject subject;
            cin >> subject.time_consuming; // time
            cin >> subject.last_subjects; // 前驱个数
            for (int j = 0; j < subject.last_subjects; j++) {
                int k;
                cin >> k;
                subject.last.push_back(k - 1);
            }
            subjects.push_back(subject);
        }
        // 手动加入结束事件
        Subject subject;
        subject.time_consuming = 0;
        subject.next_subjects = 0;
        subjects.push_back(subject);
        // 填充next的课程
        for (int i = 0; i < size; i++) {
            for (int j : subjects[i].last) {
                subjects[j].next.push_back(i);
                subjects[j].next_subjects++;
            }
        }
        // 如果没有next就指向结束事件
        for (int i = 0; i < size; i++) {
            if (subjects[i].next_subjects == 0) {
                subjects[size].last_subjects++;
                subjects[size].last.push_back(i);
                subjects[i].next_subjects++;
                subjects[i].next.push_back(size);
            }
        }
        size++;
    }

    // 计算关键路径
    bool calculateKeyWay() {
        queue<int> que;
        stack<int> sta;
        int count = 0;
        // 从无前驱科目的节点开始拓扑排序并计算min_start_time（ve）
        for (int i = 0; i < size; i++) {
            if (subjects[i].last_subjects == 0) {
                que.push(i);
            }
        }
        while (count < size && !que.empty()) {
            int j = que.front();
            que.pop();
            sta.push(j);
            count++;
            for (auto next : subjects[j].next) {
                if (--subjects[next].last_subjects == 0)
                    que.push(next);
                subjects[next].min_start_time = max(subjects[next].min_start_time, subjects[j].min_start_time + subjects[j].time_consuming);
            }
        }
        if (count != size || !que.empty()) {
            return false;
        }
        // 计算max_start_time(vl)
        for (int i = 0; i < size; i++) {
            subjects[i].max_start_time = subjects[size - 1].min_start_time;
        }
        while (!sta.empty()) {
            int j = sta.top();
            sta.pop();
            for (int i : subjects[j].next) {
                subjects[j].max_start_time = min(subjects[i].max_start_time - subjects[j].time_consuming, subjects[j].max_start_time);
            }
        }
        // e l关键路径
        for (int i = 0; i < size - 1; i++) {
            for (int j : subjects[i].next) {
                int e = subjects[i].min_start_time;
                int l = subjects[j].max_start_time - subjects[i].time_consuming;
                if (e == l) {
                    subjects[i].important = 1;
                    break;
                }
            }
        }
        return true;
    }

    // 打印答案
    void printAns() {
        if (!calculateKeyWay()) {
            cout << "error!无法拓扑排序！" << endl;
            return;
        }
        subjects.pop_back();
        for (Subject i : subjects) {
            cout << i.min_start_time + i.time_consuming << ' ' << i.important << endl;
        }
    }
};

int main() {
    Graph G;
    G.f_in();
    G.printAns();
    return 0;
}
