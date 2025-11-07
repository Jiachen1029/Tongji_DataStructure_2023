#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    // 读取数组nums和queries的长度
    cin >> n >> m;
    vector<int> nums(n);
    vector<int> queries(m);

    // 读取nums数组的元素
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    // 读取queries数组的元素
    for (int i = 0; i < m; ++i) {
        cin >> queries[i];
    }

    // 对nums进行排序，以便于找到满足条件的最长子序列
    sort(nums.begin(), nums.end());

    vector<int> answer(m);

    // 对每个查询进行处理
    for (int i = 0; i < m; ++i) {
        int sum = 0;
        int count = 0;
        // 计算nums中元素之和小于等于queries[i]的子序列的最大长度
        for (int j = 0; j < n; ++j) {
            if (sum + nums[j] <= queries[i]) {
                sum += nums[j];
                count++;
            }
            else {
                break;
            }
        }
        answer[i] = count;
    }

    // 输出结果
    for (int i = 0; i < m; ++i) {
        cout << answer[i] << " ";
    }
    cout << endl;

    return 0;
}