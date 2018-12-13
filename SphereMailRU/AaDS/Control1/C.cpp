#include <iostream>

using namespace std;

int main()
{
	long long n, x, m, s = 0, a = 1, b;
	cin >> n;
	b = 1;
	while (b * b < n)
	{
		x = b;
		b = 1 + (b << 1);
	}
	while (b - x > 1)
	{
		m = (x + b) / 2;
		if (m * m > n) b = m;
		else x = m;
	}
	s += 1 + 2 * x;
	for (long long k; (k = a * a) < n; ++a)
	{
		b = 1;
		while (b * b < n - k)
		{
			x = b;
			b = 1 + (b << 1);
		}
		while (b - x > 1)
		{
			m = (x + b) / 2;
			if (m * m > n - k) b = m;
			else x = m;
		}
		s += x;
	}
	cout << s;
	return 0;
}
