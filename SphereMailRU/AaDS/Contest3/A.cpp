#include<iostream>
#include<vector>

using namespace std;

int main()
{
	long long n, s, a, i, j;
	cin >> n;
	vector<int> arr(n);
	for (i = 0; i < n; ++i)
	{
		cin >> a;
		arr[i] = a;
	}
	s = i = 0;
	for (j = 0; j < n; ++j)
	{
		cin >> a;
		for (; i < n && arr[i] <= a; ++i)
			if (!((i + j) & 1)) s = (s + arr[i] % 1000000000) % 1000000000;
		if (!((i + j) & 1)) s = (s + a % 1000000000) % 1000000000;
	}
	for (; i < n; ++i)
		if (!((i + j) & 1)) s = (s + arr[i] % 1000000000) % 1000000000;
	cout << s;
	return 0;
}
