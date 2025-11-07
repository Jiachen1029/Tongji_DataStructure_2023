#include <iostream>
#include <stack>
#include <string>
using namespace std;

//判断给定的出栈序列是否可以通过入栈序列得到
bool isValidSequence(const string& in, const string& out) {
    stack<char> station; //车站的栈
    int j = 0; //出栈序列的指针

    //遍历入栈序列
    for (char ch : in)
    {
        station.push(ch); //将当前字符入栈，进入栈顶

        //检查栈顶元素是否与出栈序列的当前元素匹配
        while (!station.empty() && station.top() == out[j])
        {
            station.pop(); // 如果匹配则出栈
            j++; //移动出栈序列的指针
        }
    }

    // 如果栈为空，说明出栈序列是可行的
    return station.empty();
}

int main() 
{
    string in;
    cin >> in; //读取入栈序列
    string out;
    //读取多个出栈序列，直到EOF
    while (cin >> out) 
    {
        //判断当前出栈序列是否可行，并输出结果
        if (out == "EOF")
        {
            break;
        }
        else if (isValidSequence(in, out)) 
        {
            cout << "yes" << endl;
        }
        else 
        {
            cout << "no" << endl;
        }
    }

    return 0;
}