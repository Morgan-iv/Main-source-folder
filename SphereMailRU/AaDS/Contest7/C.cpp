// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end

#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<queue>

using namespace std;

//typedef pair<int, int> coord;
typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;

const int INF = 1000000000;

inline int max(int a, int b)
{
	return (a > b) ? a : b;
}

/*bool operator != (const coord & a, const coord & b)
{
	return a.first != b.first || a.second != b.second;
}

coord operator + (const coord & a, const coord & b)
{
	return make_pair(a.first + b.first, a.second + b.second);
}*/

struct edge
{
	int a, b, cost;
};

bool solve(int n, int m, int v, vector<edge> & e, vector<int> & d)
{
	d[v] = 0;
	bool any;
	for (int i = 0; i < n; ++i)
	{
		any = false;
		for (int j = 0; j < m; ++j)
			if (d[e[j].a] < INF)
				if (d[e[j].b] > d[e[j].a] + e[j].cost)
				{
					d[e[j].b] = d[e[j].a] + e[j].cost;
					any = true;
				}
		if (!any) break;
	}
	return any;
}

int main()
{
	int n, m, v;
	cin >> n >> m >> v;
	vector<int> d (n, INF);
	vector<edge> edges (m);
	for (int i = 0; i < m; ++i)
		cin >> edges[i].a >> edges[i].b >> edges[i].cost;
	if (solve(n, m, v, edges, d))
	{
		cout << "IMPOSSIBLE";
		return 0;
	}
	for (int i = 0; i < n; ++i)
	{
		if (d[i] == INF) cout << "UNREACHABLE ";
		else             cout << d[i] << " ";
	}
	return 0;
}