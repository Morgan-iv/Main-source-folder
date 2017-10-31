#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
	int n, i, s, p;
	cin >> n;
	double a;
	vector<double> arr (n);
	for(i = 0; i < n; ++i)
	{
		cin >> a;
		arr[i] = a;
	}
	sort(arr.begin(), arr.end());
	s = i = 0;
	while(i < n)
	{
		p = i;
		for(; i < n && arr[i] <= arr[p] + 1; ++i);
		++s;
	}
	cout << s;
}