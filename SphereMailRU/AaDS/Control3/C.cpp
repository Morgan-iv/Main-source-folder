// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<cstdio>
#include<queue>
#include<stack>

using namespace std;

pair<int, pair<int, int> > used[26][26];
queue<pair<int, int> > q;

int main()
{
	int n, b, y;
	pair<int, int> start, end, pos;
	stack<pair<int, int> > s;
	char a, x;
	
	scanf("%d %c%d %c%d", &n, &a, &b, &x, &y);
	a -= 'A';
	x -= 'A';
	start.first = a;
	start.second = b - 1;
	end.first = x;
	end.second = y - 1;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			used[i][j].first = -1;
		}
	}
	q.push(start);
	while (!q.empty())
	{
		pos = q.front();
		q.pop();
		if (pos.first == end.first && pos.second == end.second)
		{
			cout << used[pos.first][pos.second].first + 1 << endl;
			while (pos.first != start.first || pos.second != start.second)
			{
				s.push(pos);
				pos = used[pos.first][pos.second].second;
			}
			while (!s.empty())
			{
				cout << (char) (s.top().first + 'A') << s.top().second + 1 << endl;
				s.pop();
			}
			cout << endl;
			return 0;
		}
		for (int i = -2; i < 3; ++i)
		{
			if (i == 0) continue;
			if (pos.first + i >= n) continue;
			if (pos.first + i < 0) continue;
			for (int j = -2; j < 3; ++j)
			{
				if (j == 0) continue;
				if (pos.second + j >= n) continue;
				if (pos.second + j < 0) continue;
				if (abs(i) + abs(j) != 3) continue;
				if (used[pos.first + i][pos.second + j].first == -1)
				{
					used[pos.first + i][pos.second + j].second = pos;
					used[pos.first + i][pos.second + j].first = used[pos.first][pos.second].first + 1;
					q.push(make_pair(pos.first + i, pos.second + j));
				}
			}
		}
	}
}
