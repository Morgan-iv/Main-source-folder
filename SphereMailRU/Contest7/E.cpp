// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>

using namespace std;

struct DSU
{
	vector<pair<int, int> > st;
	int parent[1000001];
	int rank[1000001];

	void make_set (int v)
	{
		parent[v] = v;
		rank[v] = 1;
	}

	int find_set (int v)
	{
		if (v == parent[v])
			return v;
		st.push_back({ -v, parent[v] });
		return parent[v] = find_set(parent[v]);
	}

	void union_sets (int a, int b)
	{
		a = find_set(a);
		b = find_set(b);
		if (a != b)
		{
			if (rank[a] < rank[b])
				swap(a, b);
			st.push_back({ a, b });
			parent[b] = a;
			rank[a] += rank[b];
		}
	}
};

vector<vector<pair<int, int> > > edges;
int ans[1000001];
DSU sets;
int cn, cs, cnt; //-V707

int retst()
{
	pair<int, int> v;
	while ((v = sets.st[sets.st.size() - 1]).first != 0)
	{
		if (v.first < 0)
			sets.parent[-v.first] = v.second;
		else
		{
			sets.parent[v.second] = v.second;
			sets.rank[v.first] -= sets.rank[v.second];
		}
		sets.st.pop_back();
	}
	return 0;
}

int binaryadd(int l, int r)
{
	if (l == r)
		cnt += ans[l] = sets.find_set(cn) == sets.find_set(cs);
	else
	{
		int m = (l + r) / 2;
		sets.st.push_back({ 0, 0 });
		for (int i = l; i < m + 1; ++i)
		{
			for (int j = 0; j < edges[i].size(); ++j)
				sets.union_sets(edges[i][j].first, edges[i][j].second);
		}
		binaryadd(m + 1, r);
		retst();
		for (int i = m + 1; i < r + 1; ++i)
		{
			for (int j = 0; j < edges[i].size(); ++j)
				sets.union_sets(edges[i][j].first, edges[i][j].second);
		}
		binaryadd(l, m);
		retst();
		sets.st.pop_back();
	}
	return 0;
}

int main()
{
	int n, m, k, a, b, c;
	cin >> n >> m >> k;
	edges.resize(k + 1);
	cnt = 0;
	for (int i = 0; i < n; ++i)
		sets.make_set(i + 1);
	for (int i = 0; i < m; ++i)
	{
		cin >> a >> b >> c;
		if (c == 0) sets.union_sets(a, b);
		else edges[c].push_back({ a, b });
	}
	cin >> cn >> cs;
	for (int i = 0; i < n; ++i)
		sets.find_set(i + 1);
	sets.st.resize(0);
	sets.st.push_back({ 0, 0 });
	binaryadd(1, k);
	cout << cnt << endl;
	for (int i = 0; i < n; ++i)
	{
		if (ans[i]) cout << i << ' ';
	}
	return 0;
}