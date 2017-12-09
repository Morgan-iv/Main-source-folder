// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>

using namespace std;

vector<vector<pair<int, int> > > edges;
vector<char> used;
int cn, cs, cnt; //-V707

int dfs(int v, int c)
{
	used[v] = true;
	if (used[cs]) return 0;
	for (int i = 0; i < edges[v].size(); ++i)
		if (edges[v][i].second != c && !used[edges[v][i].first])
			dfs(edges[v][i].first, c);
	return 0;
}

int main()
{
	int n, m, k, a, b, c;
	cin >> n >> m >> k;
	vector<int> ans;
	edges.resize(n + 1);
	cnt = 0;
	for (int i = 0; i < m; ++i)
	{
		cin >> a >> b >> c;
		edges[a].push_back({ b, c });
		edges[b].push_back({ a, c });
	}
	cin >> cn >> cs;
	for (int i = 1; i <= k; ++i)
	{
		used.assign(n + 1, 0);
		dfs(cn, i);
		if (used[cs]) ans.push_back(i);
	}
	cout << ans.size() << endl;
	for (int i = 0; i < ans.size(); ++i)
	{
		cout << ans[i] << ' ';
	}
	return 0;
}