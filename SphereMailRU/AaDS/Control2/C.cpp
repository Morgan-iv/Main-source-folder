#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

typedef unsigned int uint;
typedef pair<string, vector<int> > rec;

vector<int> p;

bool cmpp(const rec & a, const rec & b)
{
	int i = 0;
	while (i < p.size() && a.second[p[i] - 1] == b.second[p[i] - 1])
		++i;
	return i == p.size() || a.second[p[i] - 1]  > b.second[p[i] - 1];
}

int main()
{
	int a, n, k;
	string name;
	cin >> n >> k;
	vector<rec> arr;
	for (int i = 0; i < k; ++i)
	{
		cin >> a;
		p.push_back(a);
	}
	for (int i = 0; i < n; ++i)
	{
		cin >> name;
		arr.push_back(make_pair(name, vector<int>(k)));
		for (int j = 0; j < k; ++j)
		{
			cin >> a;
			arr[i].second[j] = a;
		}
	}
	stable_sort(arr.begin(), arr.end(), &cmpp);
	for (int i = 0; i < n; ++i)
	{
		cout << arr[i].first << '\n';
	}
	return 0;
}