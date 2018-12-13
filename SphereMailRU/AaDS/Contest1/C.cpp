#include<iostream>

using namespace std;

long long powcnt(long long x, long long p, long long m)
{
	if (!p) return 1;
	if (p % 2) return (powcnt(x, p - 1, m) * x) % m;
	long long y = powcnt(x, p / 2, m);
	return (y * y) % m;
}

int main()
{
	
	long long x, p, m;
	cin >> x >> p >> m;
	if (x % m == 0)
	{
		cout << 0;
		return 0;
	}
	if (x % m == 1)
	{
		cout << 1;
		return 0;
	}
	cout << powcnt(x % m, p, m);
	return 0;
}
