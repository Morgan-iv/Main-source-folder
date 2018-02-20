#include<iostream>
#include<stack>
#include<string>
#include<stdio.h>

using namespace std;

typedef pair<char, int> oper;

void calculate(stack<oper> & op, stack<int> & val)
{
	int x, y;
	if (op.empty() || op.top().second < 2) return;
	x = val.top();
	val.pop();
	y = val.top();
	val.pop();
	if (op.top().first == '+') val.push(y + x);
	if (op.top().first == '-') val.push(y - x);
	if (op.top().first == '*') val.push(y * x);
	if (op.top().first == '/') val.push(y / x);
	op.pop();
	if (!op.empty()) ++(op.top().second);
	calculate(op, val);
}

int main()
{
	stack<oper> op;
	stack<int> val;
	string str;
	
	while (scanf("%s", str.c_str()) > 0)
	{
		if (str[0] >= '0' && str[0] <= '9')
		{
			++(op.top().second);
			val.push(atoi(str.c_str()));
			calculate(op, val);
		}
		else
		{
			op.push({ str[0], 0 });
		}
	}
	cout << val.top();
	return 0;
}