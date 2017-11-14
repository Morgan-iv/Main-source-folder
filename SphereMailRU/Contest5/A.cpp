// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<iostream>
#include<map>
#include<string>

using namespace std;

int main()
{
	int n, s, t;
	string str;
	map<string, int> arr;
	cin >> str;
	n = str.length();
	s = 0;
	for (int i = 0; i < n;)
	{
		arr.clear();
		t = 1;
		for (int j = 0; j < n - i; ++j)
			(++arr[str.substr(j, i + 1)]) > t ? (++t) : 0;
		if (s < t * (++i)) s = t * i;
	}
	cout << s;
	return 0;
}