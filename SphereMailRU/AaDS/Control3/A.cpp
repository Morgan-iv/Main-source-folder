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

vector<vector<char> > g; // граф
int nn, mm; // число вершин

vector<vector<int> > used;

void dfs(int n, int m, int c)
{
	used[n][m] = c;
	if (n > 0      && !used[n - 1][m] && g[n - 1][m] == '+') dfs(n - 1, m, c);
	if (n < nn - 1 && !used[n + 1][m] && g[n + 1][m] == '+') dfs(n + 1, m, c);
	if (m > 0      && !used[n][m - 1] && g[n][m - 1] == '+') dfs(n, m - 1, c);
	if (m < mm - 1 && !used[n][m + 1] && g[n][m + 1] == '+') dfs(n, m + 1, c);
}

int main()
{
	char c;
	
	cin >> nn >> mm;
	g.resize(nn);
	used.resize(nn);

	for (int i = 0; i < nn; ++i)
	{
		g[i].resize(mm);
		used[i].assign(mm, 0);
		for (int j = 0; j < mm; ++j)
		{
			cin >> c;
			g[i][j] = c;
		}
	}

	int count = 0;
	
	for (int i = 0; i < nn; ++i)
	{
		for (int j = 0; j < mm; ++j)
		{
			if (used[i][j] || g[i][j] == '.') continue;
			++count;
			queue<pair<int, int> > q;
			q.push({ i, j });
			used[i][j] = count;
			while (!q.empty())
			{
				pair<int, int> v = q.front();
				q.pop();
				int n = v.first;
				int m = v.second;
				if (n > 0      && !used[n - 1][m] && g[n - 1][m] == '+') { used[n - 1][m] = count; q.push({n - 1, m}); };
				if (n < nn - 1 && !used[n + 1][m] && g[n + 1][m] == '+') { used[n + 1][m] = count; q.push({n + 1, m}); };
				if (m > 0      && !used[n][m - 1] && g[n][m - 1] == '+') { used[n][m - 1] = count; q.push({n, m - 1}); };
				if (m < mm - 1 && !used[n][m + 1] && g[n][m + 1] == '+') { used[n][m + 1] = count; q.push({n, m + 1}); };
			}
		}
	}
	cout << count;
	return 0;
}
