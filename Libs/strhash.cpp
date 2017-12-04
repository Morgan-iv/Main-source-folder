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