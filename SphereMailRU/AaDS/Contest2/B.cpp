#include<iostream>
#include<vector>

using namespace std;

int main()
{
	vector<int> a, b;
	int m, n, p, i, j, as, bs;
	cin >> n >> m;
	for (i = 0; i < n; ++i)
	{
		cin >> p;
		a.push_back(p);
	}
	for (i = 0; i < m; ++i)
	{
		cin >> p;
		b.push_back(p);
	}
	p = 0;
	i = 0;
	j = 0;
	while (i < n && j < m)
	{
		if (a[i] == b[j])
		{
			++i;
			++j;
			++p;
			continue;
		}
		if (a[i] > b[j]) ++j;
		else ++i;
	}
	cout << p;
	return 0;
}