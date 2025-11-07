#include<iostream>
#include<iomanip>
using namespace std;

double salary=0;
double wholesalary=0;
int main()
{
	for (int i = 0; i < 12; i++)
	{
		cin>> salary;
		wholesalary += salary;
	}
	cout<< "¥" << fixed << setprecision(2) << wholesalary/12 << endl;
	return 0;
}