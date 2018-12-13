// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<iostream>
#include<string>

using namespace std;

int main()
{
	string str;
	cin >> str;
	int n = str.length() - 1, i, t;
	for (i = n; str[i - 1] >= str[i]; --i);
	for (t = i; t <= n && str[t] > str[i - 1]; ++t);
	n = str[i - 1];
	str[i - 1] = str[t - 1];
	str[t - 1] = n;
	for (n = str.length() - 1; i < n; ++i, --n)
	{
		t = str[i];
		str[i] = str[n];
		str[n] = t;
	}
	cout << str;
	return 0;
}
