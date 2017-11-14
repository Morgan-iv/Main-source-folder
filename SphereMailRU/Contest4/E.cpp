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

typedef pair<long long, long long> point;
typedef pair<point, long long> parr;
long long min (long long a, long long b)
{
	return (a < b) ? a : b;
}
bool pcmp (const parr & a, const parr & b)
{
	return a.first.first < b.first.first || a.first.first == b.first.first && a.first.second < b.first.second;
}
long long protate(const point a, const point b, const point c)
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
	long long n, x, y;
	scanf("%lld", &n);
	vector<parr> arr;
	for (long long i = 0; i < n; ++i)
	{
		scanf("%lld %lld", &x, &y);
		arr.push_back(make_pair(make_pair(x, y), i));
	}
	sort(arr.begin(), arr.end(), &pcmp);
	stack<parr> st;
	parr topst = arr[n - 1];
	for (long long i = n - 2; i >= 0; --i)
	{
		while (!st.empty() && protate(st.top().first, topst.first, arr[i].first) < 0)
		{
			topst = st.top();
			st.pop();
		}
		st.push(topst);
		topst = arr[i];
	}
	y = 1;
	for (long long i = 1; i < n; ++i)
	{
		while (!st.empty() && (x = protate(st.top().first, topst.first, arr[i].first)) < 0)
		{
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
	long long sz = arr.size(), i = 1, a = 0, a0 = 0, p1 = 0, p2 = 0;
	bool f1, f2, f3, f4, f5;
	for (a = 0; a < sz; ++a)
	{
		for (i = 0; i < sz; ++i)
		{
			t = len(arr[a].first, arr[i].first);
			f1 = t > s;
			f2 = t == s;
			f3 = arr[a].second < p1 || arr[i].second < p1;
			f4 = arr[a].second == p1 && arr[i].second < p2;
			f5 = arr[i].second == p1 && arr[a].second < p2;
			if (f1 || f2 && (f3 || f4 || f5))
			{
				s = t;
				p1 = arr[a].second;
				p2 = arr[i].second;
				if (p1 > p2) swap(p1, p2);
			}
			/*if ((t = len(arr[a].first, arr[i].first)) > s ||
			(t == s && (arr[a].second < p1 || arr[i].second < p1 ||
			arr[i].second == p1 && arr[a].second < p2 || arr[a].second == p1 && arr[i].second < p2)))
			{
			s = t;
			p1 = arr[a].second;
			p2 = arr[i].second;
			if (p1 > p2) swap(p1, p2);
			}*/
		}
	}
	/*if (sz == 3)
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
	}*/
	/*while (1)
	{
	x = (a + 1) % sz;
	y = (i + 1) % sz;
	n = protate(arr[a].first, arr[x].first, arr[y].first + (arr[x].first - arr[i].first));
	if ((t = len(arr[x].first, arr[y].first)) > s ||
	(t == s && (arr[x].second < p1 || arr[x].second == p1 && arr[y].second < p2)))
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
	}*/
	printf("%lld %lld", p1, p2);
	return 0;
}
