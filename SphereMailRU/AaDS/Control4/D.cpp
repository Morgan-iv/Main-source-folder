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

vector<int> p;

int dsu_get(int v)
{
	return (v == p[v]) ? v : (p[v] = dsu_get(p[v]));
}

void dsu_unite(int a, int b)
{
	a = dsu_get(a);
	b = dsu_get(b);
	if (rand() & 1)
		swap(a, b);
	if (a != b)
		p[a] = b;
}

int main()
{
	int m, n, a;
	vector < pair < int, pair<int, int> > > g; // вес - вершина 1 - вершина 2
	int cost = 0;
	vector < pair<int, int> > res;
	cin >> n;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cin >> a;
			if (a) g.push_back({ -a, {i, j} });
		}
	}
	m = g.size();
	sort(g.begin(), g.end());
	p.resize(n);
	for (int i = 0; i < n; ++i)
		p[i] = i;
	for (int i = 0; i < m; ++i)
	{
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) != dsu_get(b))
		{
			cost += l;
			res.push_back(g[i].second);
			dsu_unite(a, b);
		}
	}
	cout << -cost;
}