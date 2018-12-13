// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end

#include<iostream>
#include<string>
#include<string.h>
#include<vector>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define sz(a) (int)(a).size()

typedef long long ll;
typedef unsigned long long ull;

inline int max(int a, int b)
{
	return (a > b) ? a : b;
}

struct num
{
	static const int MA = 1e9 + 7, MB = 1e9 + 9;

	int a, b;

	//num() {}
	num(int x = 0) : a(x), b(x) {}
	num(int a, int b) : a(a), b(b) {}

	num operator + (const num &x) const { return num((a + x.a) % MA, (b + x.b) % MB); }
	num operator - (const num &x) const { return num((a + MA - x.a) % MA, (b + MB - x.b) % MB); }
	num operator * (int x) const { return num(((ll) a * x) % MA, ((ll) b * x) % MB); }
	num operator * (const num &x) const { return num(((ll) a * x.a) % MA, ((ll) b * x.b) % MB); }
	bool operator == (const num &x) const { return a == x.a && b == x.b; }

	explicit operator ll () const { return (ll) a * MB + b + 1; } // > 0
};

template <class hash_t>
struct StrComparator
{
	static const int P;
	static vector<hash_t> deg;

	int n;
	const char *s;
	hash_t *h;

	StrComparator(int n, const char *s) : n(n), s(s)
	{
		h = new hash_t[n + 1];
		h[0] = 0;
		forn(i, n)
			h[i + 1] = h[i] * P + s[i];
		deg.reserve(n);
		while (sz(deg) <= n)
			deg.push_back(*deg.rbegin() * P);
	}

	hash_t substr(int i, int len) const { return h[i + len] - h[i] * deg[len]; }

	int lcp(int i, int j)
	{
		int L = 0, R = n - max(i, j);
		while (L < R)
		{
			int M = (L + R + 1) / 2;
			if (substr(i, M) == substr(j, M))
				L = M;
			else
				R = M - 1;
		}
		return L;
	}

	int cmp(int a, int b)
	{
		int LEN = n - max(a, b), L = lcp(a, b);
		return L < LEN ? (int) s[a + L] - s[b + L] : b - a;
	}

	bool operator() (int i, int j) { return cmp(i, j) < 0; }
};
template <class hash_t> vector <hash_t> StrComparator<hash_t>::deg(1, hash_t(1));
template <class hash_t> const int StrComparator<hash_t>::P = max(239, rand());

//   StrComparator<num> h(n, s);

/**
* Usage:
*   StrComparator<num> h(length, s); // int length, char *s
*   h.substr(0, 3) == h.substr(1, 3); // сравнение на равенство подстрок за O(1)
*   h.cmp(2, 3); // сравнение на больше-меньше суффиксов за O(logn)
*
*   int p[n]; forn(i, n) p[i] = i;
*   sort(p, p + n, h); // сортировать суффиксы, суф.массив за O(nlog^2n)
*/

int main()
{
	string str;
	cin >> str;
	int p, k, n = str.length();
	ll h, m, s = 0;
	StrComparator<num> hash(n, str.c_str());
	vector<pair<ll, ll> > arr(n << 2);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n << 2; ++j)
			arr[j] = make_pair(0, 0);
		m = 0;
		for (int j = 0; j < n - i; ++j)
		{
			h = (ll)hash.substr(j, i + 1);
			p = k = h % (n << 2);
			while (arr[k].first > 0 && arr[k].first != h)
				 k = (k + 1) % (n << 2);
			arr[k].first = h;
			arr[k].second++;
			if (m < arr[k].second)
			{
				m = arr[k].second;
				if (k != p) swap(arr[p], arr[k]);
			}
		}
		s = (s > m * (i + 1)) ? s : m * (i + 1);
	}
	cout << s;
	return 0;
}
