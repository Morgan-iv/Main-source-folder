#include <iostream>
#include <string>

using namespace std;

int match(string str, string m)
{
	int a, b, i = 0, j = 0;
	while ((i < str.length()) && (m[i] != '*'))
	{
		if ((m[i] != str[i]) && (m[i] != '?')) return 0;
		++i;
	}
	j = i;
	while (i < str.length())
	{
		if (m[j] == '*')
		{
			if (++j == m.length()) return 1;
			b = j;
			a = i + 1;
		}
		else if ((j < m.length()) && ((m[j] == str[i]) || (m[j] == '?')))
		{
			++i;
			++j;
		}
		else
		{
			j = b;
			i = a++;
		}
	}
	while (m[j] == '*')
		++j;
	return j == m.length();
}

int main()
{
	string s, m;
	cin >> s;
	cin >> m;
	if (match(s, m) == 1)
		cout << "YES";
	else
		cout << "NO";
	return 0;
}
