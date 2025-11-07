#include<iostream>

using namespace std;

int main()
{
	int num = 0;//第几位字符
	int sum = 0;//总和
	char ch=getchar();//读取一个字符
	while (ch != '#')
	{
		num++;
		if (ch >= 'A' && ch <= 'Z')
		{
			sum += (ch - 'A' + 1) * num;
		}
		if (ch == '\n')//考虑换行符
		{
			if (sum!=0)
			{
				cout << sum << endl;//如果sum不为0则输出sum，否则不输出
			}
			sum = 0;
			num = 0;//重新计数
		}
		ch = getchar();
	}
	return 0;
}