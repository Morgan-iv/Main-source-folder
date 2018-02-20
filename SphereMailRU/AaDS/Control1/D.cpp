#include<iostream>
#include<vector>
#include<string>

using namespace std;

int prefix (string s, int max)
{
	int n = (int) s.length();
	vector<int> pi (n);
	for (int i = 1; i < n; ++i)
	{
		if (i == max)
		{
			pi[i] = 0;
			continue;
		}
		int j = pi[i - 1];
		while (j > 0 && s[i] != ((s[j] == '?') ? s[i] : s[j]))
			j = pi[j - 1];
		if (s[i] == ((s[j] == '?') ? s[i] : s[j]))  ++j;
		pi[i] = j;
		if ((j == max) && (i > max)) return i - max - max;
	}
	return -1;
}

int main()
{
	string s, match;
	cin >> s;
	cin >> match;
	string temp;
	int p, l = s.length(), i = 0;
	bool ast = false;
	while (i < match.length())
	{
		if (match[i] != '*') ++i;
		else
		{
			if (i == 0)
			{
				match = match.substr(1, match.length() - 1);
				ast = true;
				continue;
			}
			temp = match.substr(0, i);
			if (s.length() < i)
			{
				cout << "NO";
				return 0;
			}
			p = prefix(temp + "!" + s, i);
			if (l == s.length() && p != 0 && match[0] != '*')
			{
				cout << "NO";
				return 0;
			}
			if (p == -1)
			{
				cout << "NO";
				return 0;
			}
			s = s.substr(p + i, s.length() - (p + i));
			match = match.substr(i + 1, match.length() - i - 1);
			i = 0;
			ast = false;
		}
	}
	if (ast)
	{
		cout << "YES";
		return 0;
	}
	if (match.length() > s.length())
	{
		cout << "NO";
		return 0;
	}
	p = prefix(match + "!" + s, match.length());
	if (p == -1)
	{
		cout << "NO";
		return 0;
	}
	if (l == s.length() && p != 0 && match[0] != '*')
	{
		cout << "NO";
		return 0;
	}
	cout << ((p + match.length() == s.length()) ? "YES" : "NO");
	return 0;
}