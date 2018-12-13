// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end

#include<iostream>
#include<string>
#include<string.h>
#include<vector>

using namespace std;

typedef long long int64;
typedef unsigned long long uint64;

inline int max(int a, int b)
{
	return (a > b) ? a : b;
}

void dfs (int v, vector<bool> & used, vector<vector<int> > & g)
{
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); ++i)
	{
		int to = g[v][i];
		if (!used[to])
			dfs(to, used, g);
	}
}

int main()
{
	int n, m, a, b;
	cin >> n >> m;
	vector<bool> edges(n);
	vector<vector<int> > graph (n);
	bool s = false;
	for (int i = 0; i < m; ++i)
	{
		cin >> a >> b;
		edges[a - 1] = !edges[a - 1];
		edges[b - 1] = !edges[b - 1];
		graph[a - 1].push_back(b - 1);
		graph[b - 1].push_back(a - 1);
	}
	for (int i = 0; i < n; ++i)
	{
		s = s || edges[i];
	}
	if (s)
	{
		cout << "NO";
		return 0;
	}
	s = true;
	dfs(0, edges, graph);
	for (int i = 0; i < n; ++i)
	{
		s = s && edges[i];
	}
	cout << (s ? "YES" : "NO");
	return 0;
}
