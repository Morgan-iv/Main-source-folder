// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<iostream>
#include<vector>
#include<string>

#define kektor vector
//))))))))))))))))

using namespace std;

kektor<kektor<int> > arr;

int min(int a, int b, int c)
{
	return (a < b) ? (a < c) ? a : c : (b < c) ? b : c;
}

/*int print()
{
	for (int i = 0; i < arr.size(); ++i)
	{
		for (int j = 0; j < arr[i].size(); ++j)
		{
			cout << arr[i][j] << ' ';
		}
		cout << '\n';
	}
	return 0;
}*/

int main()
{
	string s1, s2;
	cin >> s1;
	cin >> s2;
	arr.resize(s2.length() + 1);
	arr[0].resize(s1.length() + 1);
	for (int i = 0; i < s1.length() + 1; ++i)
		arr[0][i] = i;
	for (int i = 1; i < s2.length() + 1; ++i)
	{
		arr[i].resize(s1.length() + 1);
		arr[i][0] = i;
	}
	for (int i = 1; i < s2.length() + 1; ++i)
		for (int j = 1; j < s1.length() + 1; ++j)
			arr[i][j] = min(arr[i][j - 1] + 1, arr[i - 1][j] + 1, arr[i - 1][j - 1] + (s1[j - 1] != s2[i - 1]));
	cout << arr[s2.length()][s1.length()];
}
