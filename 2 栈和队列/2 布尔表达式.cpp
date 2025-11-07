#include<iostream>
#include<stack>
#include<string>

using namespace std;

//T，F所成的栈
stack<bool> boolStack;
//操作符所成的栈
stack<char> opStack;

void processOperator() {
    char op = opStack.top();
    opStack.pop();
    if (op == '|') {
        bool b1 = boolStack.top();
        boolStack.pop();
        bool b2 = boolStack.top();
        boolStack.pop();
        boolStack.push(b1 || b2);
    }
    else if (op == '&') {
        bool b1 = boolStack.top();
        boolStack.pop();
        bool b2 = boolStack.top();
        boolStack.pop();
        boolStack.push(b1 && b2);
    }
    else if (op == '!') {
        bool b = boolStack.top();
        boolStack.pop();
        boolStack.push(!b);
    }
}

//读取输入的字符串
void procession(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == 'V') //读到V，入栈boolStack
        {
            boolStack.push(true);
        }
        else if (str[i] == 'F') //读到F，入栈boolStack
        {
            boolStack.push(false);
        }
        else if (str[i] == '(') //读到(，入栈opStack
        {
            opStack.push(str[i]);
        }
        else if (str[i] == ')') //读到)，计算括号内的表达式
        {
            while (opStack.top() != '(')
            {
                processOperator();
            }
            opStack.pop(); //弹出左括号
        }
        else if (str[i] == '|') //读到|，入栈opStack
        {
            while (!opStack.empty() && opStack.top() != '(' && opStack.top() != '|')
            {
                processOperator();
            }
            opStack.push(str[i]);
        }
        else if (str[i] == '&') //读到&，判断栈顶是否为|，如果是则先计算|，再入栈&
        {
            while (!opStack.empty() && opStack.top() != '(' && opStack.top() != '|' && opStack.top() != '&')
            {
                processOperator();
            }
            opStack.push(str[i]);
        }
        else if (str[i] == '!') //读到!，立即处理
        {
            opStack.push(str[i]);
        }
        else if (str[i] == ' ') //读到空格，跳过
        {
            continue;
        }
    }
    while (!opStack.empty()) //处理剩余的操作符
    {
        processOperator();
    }
}

//主函数，循环读取字符串，并输出结果
int main()
{
    string str;
    int index = 1;
    while (getline(cin, str))
    {
        procession(str);
        if (boolStack.top())
        {
            cout << "Expression " << index << ": V" << endl;
        }
        else
        {
            cout << "Expression " << index << ": F" << endl;
        }
        //清空栈以处理下一个表达式
        while (!boolStack.empty()) boolStack.pop();
        while (!opStack.empty()) opStack.pop();
        index++;
    }
    return 0;
}