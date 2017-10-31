#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

int main()
{
	int n, a, b;
	cin >> n;
	vector<pair<int, int> > arr (n);
	vector<int> str (1);
	for (int i = 0; i < n; ++i)
	{
		cin >> a >> b;
		arr[i] = make_pair(b, a);
	}
	sort(arr.begin(), arr.end());
	str[0] = arr[0].first;
	for (int i = 1; i < n; ++i)
	{
		a = str.size();
		b = 1;
		for (int j = 0; j < a; ++j)
		{
			if (str[j] < arr[i].second)
			{
				str[j] = arr[i].first;
				b = 0;
				break;
			}
		}
		if (b) str.push_back(arr[i].first);
	}
	cout << str.size();
	return 0;
}