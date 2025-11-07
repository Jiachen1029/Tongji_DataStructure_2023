#include <iostream>
using namespace std;

int josephus(int n, int m)
{
    int result = 0; // 初始情况下，只有一个人时，幸存者编号为0
    for (int i = 2; i <= n; i++)
    {
        result = (result + m) % i;
    }
    return result;
}
int main()
{
    int n, m;
    cin >> n >> m; // 输入人数和报数的数字
    cout << josephus(n, m) << endl;
    return 0;
}