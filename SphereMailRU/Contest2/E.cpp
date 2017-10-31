#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
	const int maxelem = 20010000;
	int m, k = 0, i, n, b, c = 1;
	cin >> m >> n;
	if (m > n)
	{
		cout << -1;
		return 0;
	}
	vector<char> prime(maxelem + 1, true);
	prime[0] = prime[1] = false;
	for (i = 2; i <= maxelem; ++i)
		if (prime[i])
			if (i * 1ll * i <= maxelem)
				for (int j = i * i; j <= maxelem; j += i)
					prime[j] = false;
	vector<char> bitset((maxelem + 10) / 8, false);
	for (i = 0; i < maxelem + 1; ++i)
	{
		bitset[i / 8] = bitset[i / 8] | (prime[i] << i % 8);
	}
	vector<char>().swap(prime);
	vector<int> intpr(1);
	intpr[0] = 1;
	for (i = 0; i < maxelem + 1; ++i)
	{
		if (bitset[i / 8] & 1 << i % 8) intpr.push_back(i);
		if (c && i > 20000000)
		{
			c = 0;
			b = intpr.size();
		}
	}
	int sz = intpr.size();
	for (i = 1, c = m; i < b; ++i, ++c)
	{
		if (c + 1 > sz)
		{
			cout << -1;
			return 0;
		}
		if (intpr[c] >= intpr[i] + n) continue;
		k = (intpr[c + 1] >= intpr[i] + n) ? ((intpr[c] - n + 1 > intpr[i - 1] + 1) ? intpr[c] - n + 1 : intpr[i - 1] + 1) : (intpr[c + 1] >= intpr[i - 1] + 1 + n) ? ((intpr[c] - n + 1 > intpr[i - 1] + 1) ? intpr[c] - n + 1 : intpr[i - 1] + 1) : 0;
		if (k) break;
	}
	if (!k)
	{
		cout << -1;
		return 0;
	}
	cout << k;
	return 0;
}