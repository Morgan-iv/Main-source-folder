// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

int main()
{
	int n, m, a, s = 0, t;
	cin >> n >> m;
	vector<vector<int> > arrs(n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			cin >> a;
			arrs[i].push_back(a);
		}
		sort(arrs[i].begin(), arrs[i].end());
	}
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
		{
			t = 0;
			for (int p1 = 0, p2 = 0; p1 < m && p2 < m;)
				(arrs[i][p1] < arrs[j][p2]) ? ++p1 : ((arrs[i][p1] > arrs[j][p2]) ? ++p2 : (++t, ++p1, ++p2));
			if (s < t) s = t;
		}
	cout << s;
	return 0;
}
