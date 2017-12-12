// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<cstdio>
#include<stack>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<string>

using namespace std;

typedef unsigned long long uint64;

template<typename T>
class CRCCoder
{
public:
	CRCCoder()
	{
		init();
	}

	static T calc(const void* data, unsigned size)
	{
		initTable();
		const unsigned char *p = reinterpret_cast<const unsigned char *>(data);
		T ctmp = T(-1);
		while (size)
		{
			ctmp ^= (T) (p[0]);
			ctmp = (ctmp >> 8) ^ _table[ctmp & 0xFF];
			p++;
			size--;
		}
		return ctmp ^ T(-1);
	}

	void init()
	{
		initTable();
		_value = T(-1);
	}

	void update(unsigned char p)
	{
		_value ^= (T) (p);
		_value = (_value >> 8) ^ _table[_value & 0xFF];
	}

	T final()
	{
		return _value ^ T(-1);
	}

private:
	T _value;
	static T _table[256];
	static T POLY;
	static void initTable()
	{
		if (_table[0] == 0)
		{
			T *p = _table;
			int             i, j;
			for (j = 0; j < 256; j++)
			{
				T x = j;
				for (i = 0; i < 8; i++)
				{
					if ((x & 1) != 0)
					{
						x = (x >> 1) ^ 0xC96C5795D7870F42ULL;
					}
					else
					{
						x = (x >> 1);
					}
				}
				*p++ = x;
			}
		}
	}
};

template<> unsigned long long CRCCoder<unsigned long long>::POLY = 0xC96C5795D7870F42ULL;
template<> unsigned CRCCoder<unsigned>::POLY = 0xEDB88320U;
template<> unsigned long long CRCCoder<unsigned long long>::_table[256] = { 0 };
template<> unsigned CRCCoder<unsigned>::_table[256] = { 0 };

int cmp(const void * a, const void * b)
{
	return (*((int *) a)) < (*((int *) b));
}

int main()
{
	CRCCoder<uint64> coder;

	int n, m, k, a;
	scanf("%d %d %d", &n, &m, &k);
	vector<pair<uint64, vector<int>> > sets(n), fnd(k);
	set<uint64> sets2;
	for (int i = 0; i < n; ++i)
	{
		sets[i].second.resize(m);
		for (int j = 0; j < m; ++j)
		{
			scanf("%d", &(sets[i].second[j]));
		}
		sort(sets[i].second.begin(), sets[i].second.end());
		sets[i].first = coder.calc(sets[i].second.data(), sizeof(int) * m);
		sets2.insert(sets[i].first);
	}
	for (int i = 0; i < k; ++i)
	{
		fnd[i].second.resize(m);
		for (int j = 0; j < m; ++j)
		{
			scanf("%d", &(fnd[i].second[j]));
		}
		sort(fnd[i].second.begin(), fnd[i].second.end());
		fnd[i].first = coder.calc(fnd[i].second.data(), sizeof(int) * m);
	}
	for (int i = 0; i < k; ++i)
	{
		a = sets2.find(fnd[i].first) != sets2.end();
		cout << a << endl;
	}
	return 0;
}