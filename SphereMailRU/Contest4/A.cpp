// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
	int n, a, m;
	cin >> n;
	vector<pair<int, int> > arr(n);
	for (int i = 0; i < n; ++i)
	{
		cin >> a;
		arr[i] = make_pair(a, i + 1);
	}
	sort(arr.begin(), arr.end());
	m = 2000000005;
	for (int i = 1; i < n; ++i)
		if (arr[i].first - arr[i - 1].first < m)
		{
			a = i;
			m = arr[i].first - arr[i - 1].first;
		}
	cout << m << endl <<
		((arr[a - 1].second > arr[a].second) ? arr[a].second : arr[a - 1].second) << ' ' <<
		((arr[a - 1].second > arr[a].second) ? arr[a - 1].second : arr[a].second);
	return 0;
}