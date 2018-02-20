// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>
#include<set>

#define kektor vector
//))))))))))))))))

using namespace std;

kektor<kektor<char> > g; //char
kektor<char> used;
kektor<int> order;
set<int>component;
int n; //-V707

void dfs1(int v)
{
	used[v] = true;
	for (int i = 0; i < n; ++i)
	{
		if (!g[v][i]) continue;
		if (!used[i])
			dfs1(i);
	}
	order.push_back(v);
}

void dfs2(int v)
{
	used[v] = true;
	component.insert(v);
	for (int i = 0; i < n; ++i)
	{
		if (!g[i][v]) continue;
		if (!used[i])
			dfs2(i);
	}
}

int main()
{
	int a, rc = 0, lc = 0;
	cin >> n;
	g.resize(n);
	for (int i = 0; i < n; ++i)
	{
		g[i].resize(n);
		for (int j = 0; j < n; ++j)
		{
			cin >> a;
			g[i][j] = a;
		}	
	}
	used.assign(n, false);
	for (int i = 0; i < n; ++i)
		if (!used[i])
			dfs1(i);
	used.assign(n, false);
	for (int i = 0; i < n; ++i)
	{
		int v = order[n - 1 - i];
		if (!used[v])
		{
			dfs2(v);
			int root = 1, leaf = 1;
			for (auto iter = component.begin(); iter != component.end(); ++iter)
			{
				for (int j = 0; j < n; ++j)
				{
					leaf = leaf && !(g[*iter][j] && component.find(j) == component.end());
					root = root && !(g[j][*iter] && component.find(j) == component.end());
				}
			}
			rc += root;
			lc += leaf;
			component.clear();
		}
	}
	cout << ((rc > lc) ? rc : lc);
	return 0;
}