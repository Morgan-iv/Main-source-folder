#include<cstdio>
#include<vector>
#include<algorithm>
#include<stack>
#include<math.h>

using namespace std;

typedef pair<int, int> point;

bool pcmp (point a, point b)
{
	return a.first < b.first || a.first == b.first && a.second < b.second;
}
int protate(point a, point b, point c)
{
	return (b.first - a.first) * (c.second - b.second) - (b.second - a.second) * (c.first - b.first);
}
double len(point a, point b)
{
	return sqrt((b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second));
}

int main()
{
	int n, x, y;
	scanf("%d", &n);
	vector<point> arr;
	for (int i = 0; i < n; ++i)
	{
		scanf("%d %d", &x, &y);
		arr.push_back(make_pair(x, y));
	}
	sort(arr.begin(), arr.end(), &pcmp);
	stack<point> st;
	point topst = arr[n - 1];
	for (int i = n - 2; i >= 0; --i)
	{
		while (!st.empty() && protate(st.top(), topst, arr[i]) < 0)
		{
			topst = st.top();
			st.pop();
		}
		st.push(topst);
		topst = arr[i];
	}
	for (int i = 1; i < n; ++i)
	{
		while (!st.empty() && protate(st.top(), topst, arr[i]) < 0)
		{
			topst = st.top();
			st.pop();
		}
		st.push(topst);
		topst = arr[i];
	}
	double s = 0;
	while (!st.empty())
	{
		s += len(st.top(), topst);
		topst = st.top();
		st.pop();
	}
	printf("%.2f", s);
	return 0;
}