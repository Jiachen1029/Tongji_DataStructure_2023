/**
 * @file    template.cpp
 * @name    模板程序
 * @date    2022-11-22
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
    std::string largestNumber(std::vector<int>& nums)
    {
        // 这里填写你的代码 
		//保证第一位要最大
        sort(nums.begin(), nums.end(), [](const int& x, const int& y)
            {
            return to_string(x) + to_string(y) > to_string(y) + to_string(x);
            });
        if (nums[0] == 0)
        {
            return "0";
        }
        string ret;
        for (int& x : nums)
        {
            ret += to_string(x);
        }
        return ret;
    }
};
/********************************/
/*     以上是你需要提交的代码     */
/********************************/

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for (int i = 0; i < n; i ++ ) {
        std::cin >> nums[i];
    }
    Solution s;
    std::cout << s.largestNumber(nums) << std::endl;
    return 0;
}