// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<cstdio>
#include<vector>
#include<algorithm>
#include<stack>
#include<math.h>

using namespace std;

typedef pair<int, int> point;
typedef pair<point, int> parr;
int min (int a, int b)
{
	return (a < b) ? a : b;
}
bool pcmp (const parr & a, const parr & b)
{
	return a.first.first < b.first.first || a.first.first == b.first.first && a.first.second < b.first.second;
}
int protate(const point a, const point b, const point c)
{
	return (b.first - a.first) * (c.second - b.second) - (b.second - a.second) * (c.first - b.first);
}
double len(const point a, const point b)
{
	return sqrt((b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second));
}
point operator + (const point & a, const point & b)
{
	return make_pair(a.first + b.first, a.second + b.second);
}
point operator - (const point & a)
{
	return make_pair(-a.first, -a.second);
}
point operator - (const point & a, const point & b)
{
	return a + -b;
}

int main()
{
	int n, x, y;
	scanf("%d", &n);
	vector<parr> arr;
	for (int i = 0; i < n; ++i)
	{
		scanf("%d %d", &x, &y);
		arr.push_back(make_pair(make_pair(x, y), i));
	}
	sort(arr.begin(), arr.end(), &pcmp);
	stack<parr> st;
	parr topst = arr[n - 1];
	for (int i = n - 2; i >= 0; --i)
	{
		while (!st.empty() && protate(st.top().first, topst.first, arr[i].first) <= 0)
		{
			topst = st.top();
			st.pop();
		}
		st.push(topst);
		topst = arr[i];
	}
	y = 1;
	for (int i = 1; i < n; ++i)
	{
		while (!st.empty() && (x = protate(st.top().first, topst.first, arr[i].first)) <= 0)
		{ 
			if ((y = y && !x)) break;
			topst = st.top();
			st.pop();
		}
		st.push(topst);
		topst = arr[i];
	}
	double t, s = 0;
	arr.resize(0);
	while (!st.empty())
	{
		arr.push_back(topst);
		topst = st.top();
		st.pop();
	}
	int sz = arr.size(), i = 1, a = 0, a0 = 0, p1 = 0, p2 = 0;
	if (sz == 3)
	{
		double s1, s2, s3;
		s1 = len(arr[0].first, arr[1].first); //-V525
		s2 = len(arr[0].first, arr[2].first); //-V525
		s3 = len(arr[1].first, arr[2].first); //-V525
		if (s1 > s2)
			if (s1 > s3)
				p1 = arr[0].second, p2 = arr[1].second;
			else
				if (s1 < s3)
					p1 = arr[1].second, p2 = arr[2].second;
				else
					p1 = arr[1].second, p2 = min(arr[2].second, arr[0].second);
		else
			if (s1 < s2)
				if (s2 > s3)
					p1 = arr[0].second, p2 = arr[2].second;
				else
					if (s2 < s3)
						p1 = arr[1].second, p2 = arr[2].second;
					else
						p1 = arr[2].second, p2 = min(arr[0].second, arr[1].second);
			else
				if (s2 < s3)
					p1 = arr[1].second, p2 = arr[2].second;
				else
					if (s2 > s3)
						p1 = arr[0].second, p2 = min(arr[2].second, arr[1].second);
					else
						p1 = 0, p2 = 1;
		if (p1 > p2) swap(p1, p2);
		printf("%d %d", p1, p2);
		return 0;
	}
	while (1)
	{
		x = (a + 1) % sz;
		y = (i + 1) % sz;
		n = protate(arr[a].first, arr[x].first, arr[y].first + (arr[x].first - arr[i].first));
		if ((t = len(arr[x].first, arr[y].first)) > s ||
			(t == s && (arr[x].second < p1 || arr[y].second < p1)))
		{
			s = t;
			p1 = arr[x].second;
			p2 = arr[y].second;
			if (p1 > p2) swap(p1, p2);
		}
		if (n > 0) a = x, a0 = 1;
		if (n < 0) i = y;
		if (n == 0) a = x, i = y;
		if (a == 0 && a0) break;
	}
	printf("%d %d", p1, p2);
	return 0;
}