#include<iostream>

using namespace std;

char getsym(int n, int k, int s)
{
	if (k == 0) return 'a' + n - 1;
	return getsym(n - 1, (k - 1) % s, (s - 1) / 2);
}

int main()
{
	int l, k, s = 1, n;
	cin >> n >> k >> l;
	for (int i = 2; i < n; ++i) s = 2 * s + 1;
	for (int i = k - 1; i < l; ++i) cout << getsym(n, i, s);
	return 0;
}