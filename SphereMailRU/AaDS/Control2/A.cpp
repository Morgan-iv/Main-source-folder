#include<iostream>
#include<vector>

using namespace std;

typedef unsigned int uint;

int main()
{
	int m, n, k, a, l, r, s;
	cin >> n >> k;
	vector<uint> arr(n);
	for (int i = 0; i < n; ++i)
	{
		cin >> a;
		arr[i] = a;
	}
	s = 1;
	l = 0;
	while (s)
	{
		r = 0;
		for (int i = 0; i < n; ++i)
		{
			r += arr[i] >> l;
			s = r >= k;
			if (s) break;
		}
		++l;
	}
	r = 1 << (l - 1);
	l = 1 << (l - 2);
	while (r - l > 1)
	{
		m = (l + r) / 2;
		s = 0;
		for (int i = 0; i < n; ++i)
		{
			s += arr[i] / m;
			if (s >= k) break;
		}
		if (s < k) r = m;
		else l = m;
	}
	cout << l;
	return 0;
}