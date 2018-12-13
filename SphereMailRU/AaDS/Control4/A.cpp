// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<cstdio>
#include<stack>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<string>

using namespace std;

bool cmp(const pair<pair<int, int>, string> & p, const pair<pair<int, int>, string> & q)
{
	return p.first.first < q.first.first || p.first.first == q.first.first && p.first.second > q.first.second;
}

int main()
{
	int n, m, a;
	string str;
	cin >> m >> n;
	map<string, pair<int, int> > res;
	for (int i = 0; i < n; ++i)
	{
		cin >> str >> a;
		if (res[str].first < a) res[str] = { a, i };
	}
	vector<pair<pair<int, int>, string > > table(m);
	a = 0;
	for (auto iter = res.begin(); iter != res.end(); ++iter, ++a)
	{
		table[a] = { (*iter).second, (*iter).first };
	}
	sort(table.begin(), table.end(), cmp);
	for (int i = table.size() - 1; i >= 0; --i)
	{
		cout << table[i].second << endl;
	}
}
