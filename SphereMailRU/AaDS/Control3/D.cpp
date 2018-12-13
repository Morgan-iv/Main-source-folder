// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<cstdio>
#include<queue>
#include<stack>
#include<vector>
#include<set>
#include<map>
#include<algorithm>

using namespace std;

const int INF = 1000000000;

int main()
{
	int n, m, k, s, e, a, b, c;
	cin >> n >> m >> k;
	vector<map<int, int> > g(n);
	vector<int> fires(k);
	for (int i = 0; i < k; ++i)
	{
		cin >> fires[i];
		--fires[i];
	}
	for (int i = 0; i < m; ++i)
	{
		cin >> a >> b >> c;
		--a;
		--b;
		if (g[a].find(b) == g[a].end() || g[a][b] > c)
		{
			g[a][b] = c;
			g[b][a] = c;
		}
	}
	cin >> s >> e; // стартовая вершина
	--s;
	--e;
	
	vector<int> d(n, INF);
	d[s] = 0;
	set < pair<int, int> > q;
	q.insert(make_pair(d[s], s));
	while (!q.empty())
	{
		int v = q.begin()->second;
		q.erase(q.begin());
		
		for (auto iter = g[v].begin(); iter != g[v].end(); ++iter)
		{
			int to = (*iter).first,
				len = (*iter).second;
			if (d[v] + len < d[to])
			{
				q.erase(make_pair(d[to], to));
				d[to] = d[v] + len;
				q.insert(make_pair(d[to], to));
			}
		}
	}
	a = d[e];
	if (a == INF)
	{
		cout << -1;
		return 0;
	}

	for (int i = 0; i < k; ++i)
	{
		d.assign(n, INF);
		d[fires[i]] = 0;
		q.insert(make_pair(d[fires[i]], fires[i]));
		while (!q.empty())
		{
			int v = q.begin()->second;
			q.erase(q.begin());

			for (auto iter = g[v].begin(); iter != g[v].end(); ++iter)
			{
				int to = (*iter).first,
					len = (*iter).second;
				if (d[v] + len < d[to])
				{
					q.erase(make_pair(d[to], to));
					d[to] = d[v] + len;
					q.insert(make_pair(d[to], to));
				}
			}
		}
		if (a > d[e])
		{
			cout << -1;
			return 0;
		}
	}
	cout << a;
	return 0;
}
