#include <iostream>
#include <stack>
#include <string>
#include <tuple>
using namespace std;

tuple<int, int> LongestValidParentheses(const string& s)//返回最长有效括号的长度和起始位置
{
    int maxLength = 0;
    int startPosition = 0;
    stack<int> stack;
    stack.push(-1);

    for (int i = 0; i < s.length(); ++i) 
    {
        if (s[i] == '(') 
        {
            stack.push(i);//输入的是左括号，将其下标入栈
        }
        else 
        {
            stack.pop();//输入的是右括号，将栈顶元素出栈
            if (stack.empty()) 
            {
                stack.push(i);//栈为空，将当前右括号的下标入栈
            }
            else
            {
                int length = i - stack.top();
                if (length > maxLength)
                {
                    maxLength = length;
                    startPosition = stack.top() + 1;
                }
            }
        }
    }

    return make_tuple(maxLength, startPosition);//返回最长有效括号的长度和起始位置
}

int main()
{
    string input;
    cin >> input;
    auto result = LongestValidParentheses(input);
    cout << get<0>(result) << " " << get<1>(result) << endl;
    return 0;
}