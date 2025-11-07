#include<iostream>
using namespace std;

long long man=0;
long long woman = 0;
int king = 1;
int year;
long long nman, nwoman;//下一年的雄和雌

int main()
{
	cin >> year;
	while (year != -1)
	{
		for (int i = 0; i < year; i++)
		{
			nman = man + woman + king;
			nwoman = man;
			man = nman;
			woman = nwoman;//下一年重新赋值
		}
		cout << man << ' ' << (woman + man + king) << endl;
		//重置
		man = 0, woman = 0;
		cin >> year;
	}
	return 0;
}