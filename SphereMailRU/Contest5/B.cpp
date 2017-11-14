// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<iostream>
#include<map>
#include<string>
#include<vector>

using namespace std;

struct station
{
	int x;
	int y;
	int r;
	int b;
	string name;
	station(int _x = 0, int _y = 0, int _r = 0, int _b = 1, string _name = "") : x(_x), y(_y), r(_r), b(_b), name(_name) { }
};

int main()
{
	int n, x, y, r, t;
	string str;
	cin >> n;
	map<string, int> count;
	vector<station> arr;
	for (int i = 0; i < n; ++i)
	{
		cin >> str;
		cin >> x >> y >> r;
		arr.push_back({ x, y, r, 1, str });
	}
	cin >> x >> y;
	for (int i = 0; i < n; ++i)
	{
		t = (x - arr[i].x) * (x - arr[i].x) +
			(y - arr[i].y) * (y - arr[i].y) <=
				  arr[i].r * arr[i].r;
		if (count.find(arr[i].name) != count.end()) arr[i].b = 0;
		count[arr[i].name] += t;
	}
	cout << count.size() << '\n';
	for (int i = 0; i < n; ++i)
	{
		if (arr[i].b) cout << arr[i].name << ' ' << count[arr[i].name] << '\n';
	}
	return 0;
}