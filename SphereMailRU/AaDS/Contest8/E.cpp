// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

#define kektor vector
//))))))))))))))))

using namespace std;

kektor<int> p;

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
	int n, m, a, b, c;
	kektor<pair<int, pair<int, int> > > g; // вес - вершина 1 - вершина 2
	cin >> n >> m;
	p.resize(n);
	for (int i = 0; i < m; ++i)
	{
		cin >> a >> b >> c;
		g.push_back(make_pair(c, make_pair(a - 1, b - 1)));
	}
	int cost = 0;
	kektor<pair<int, int> > res;
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
	cout << cost;
	return 0;
}
