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

vector<int> pi;

int prefix_function(string s)
{
	int n = (int) s.length();
	pi.assign(n, 0);
	int max = 0;
	for (int i = 1; i < n; ++i)
	{
		int j = pi[i - 1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
		max = (max > j) ? max : j;
	}
	return max;
}

int main()
{
	string s1, s2, s, res, t;
	int m = 0, n, k;
	vector<int> pos;
	cin >> s1 >> s2;
	s = s1 + '#' + s2;
	for (int i = 0; i < s1.length(); ++i)
	{
		n = prefix_function(s.substr(i, s.length() - i));
		if (n > m)
		{
			m = n;
			pos.assign(1, i);
		}
		else if (n == m)
		{
			pos.push_back(i);
		}
	}
	for (int i = 0; i < pos.size(); ++i)
	{
		t = s.substr(pos[i], m);
		if (res == "" || res > t) res = t;
	}
	cout << res;
}
