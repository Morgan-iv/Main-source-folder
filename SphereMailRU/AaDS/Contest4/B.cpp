// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<iostream>
#include<vector>

using namespace std;

int main()
{
	int n, x, a, b, m;
	cin >> n;
	vector<vector<int> > arr(n);
	for (int i = 0; i < n; ++i)
	{
		arr[i] = vector<int>(n);
		for (int j = 0; j < n; ++j)
		{
			cin >> x;
			arr[i][j] = x;
		}
	}
	cin >> x;
	for (int i = 0; i < n; ++i)
	{
		a = 0;
		b = n - 1;
		while (a != b)
		{
			m = (a + b) / 2;
			if (x > arr[i][m]) a = m + 1;
			else               b = m;
		}
		if (arr[i][a] == x)
		{
			cout << "yes";
			return 0;
		}
	}
	cout << "no";
	return 0;
}
