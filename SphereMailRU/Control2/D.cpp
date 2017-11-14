//#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

typedef unsigned int uint;
typedef long long int64;

int prefix_function (string s)
{
	int n = (int) s.length();
	vector<int> pi (n);
	for (int i = 1; i<n; ++i)
	{
		int j = pi[i - 1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
	return pi[n - 1];
}

int main()
{
	string s;
	cin >> s;
	cout << s.length() - prefix_function(s);
}