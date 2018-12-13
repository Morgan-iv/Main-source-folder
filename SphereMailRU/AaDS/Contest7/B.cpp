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

typedef pair<int, int> coord;
typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;

inline int max(int a, int b)
{
	return (a > b) ? a : b;
}

bool operator != (const coord & a, const coord & b)
{
	return a.first != b.first || a.second != b.second;
}

coord operator + (const coord & a, const coord & b)
{
	return make_pair(a.first + b.first, a.second + b.second);
}

int main()
{
	int n, m;
	string str;
	coord start, tmp, end/*, m1, m2, p1, p2*/;
	cin >> n >> m;
	vector<vector<int> > maze(n);
	queue<coord> q;
	for (int i = 0; i < n; ++i)
	{
		maze[i].resize(m);
		cin >> str;
		for (int j = 0; j < m; ++j)
		{
			maze[i][j] = (str[j] == '+') ? -1 : 0;
			if (str[j] == '@')
			{
				start = make_pair(i, j);
				maze[i][j] = 1;
			}
			if (str[j] == '#')
				end = make_pair(i, j);
		}
	}
	
	q.push(start);
	while (start != end)
	{
		if (start.first > 0      && maze[start.first - 1][start.second] == 0)
		{
			tmp = make_pair(start.first - 1, start.second);
			q.push(tmp);
			maze[start.first - 1][start.second] = maze[start.first][start.second] + 1;
		}
		if (start.first < n - 1  && maze[start.first + 1][start.second] == 0)
		{
			tmp = make_pair(start.first + 1, start.second);
			q.push(tmp);
			maze[start.first + 1][start.second] = maze[start.first][start.second] + 1;
		}
		if (start.second > 0     && maze[start.first][start.second - 1] == 0)
		{
			tmp = make_pair(start.first, start.second - 1);
			q.push(tmp);
			maze[start.first][start.second - 1] = maze[start.first][start.second] + 1;
		}
		if (start.second < m - 1 && maze[start.first][start.second + 1] == 0)
		{
			tmp = make_pair(start.first, start.second + 1);
			q.push(tmp);
			maze[start.first][start.second + 1] = maze[start.first][start.second] + 1;
		}
		q.pop();
		start = q.front();
	}
	cout << maze[end.first][end.second] - 1;

	return 0;
}
