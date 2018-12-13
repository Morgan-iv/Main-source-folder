#include<iostream>
#include<vector>

//using namespace std;

int main()
{
	int arr[8];
	int c, k, p = 1;
	int me, mp;
	for (int i = 0; i < 8; ++i)
	{
		std::cin >> c;
		arr[i] = c;
	}
	std::cin >> c;
	me = arr[0];
	mp = me * c;
	arr[0] = c;
	while (std::cin >> c)
	{
		if (c <= 0)
		{
			std::cout << mp;
			return 0;
		}
		me = (me > arr[p]) ? me : arr[p];
		k = c * me;
		mp = (mp > k) ? mp : k;
		arr[p] = c;
		p = (p + 1) & 7;
	}
	return 0;
}
