// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>

using namespace std;

typedef unsigned int uint;

int main()
{
	int n, m;
	cin >> n >> m;
	vector<uint> arr(n + 1);
	arr[0] = 1;
	for (int i = 1; i <= n; ++i)
		for (int j = 1; i - j >= 0 && j <= m; ++j)
			arr[i] += arr[i - j];
	cout << arr[n];
	return 0;
}