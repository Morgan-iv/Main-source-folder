#include<iostream>

using namespace std;

int main()
{
	int sum = 0, n, a;
	cin >> n;
	for (int i = 0; i < n; ++i)
	{
		cin >> a;
		if (a > sum + 1) break;
		sum += a;
	}
	cout << sum + 1;
}