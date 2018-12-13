#include<iostream>
#include<vector>

using namespace std;

typedef unsigned int uint;

int main()
{
	int n;
	const uint block = 256;
	const uint bllen = 8;
	uint k, m, l;
	unsigned long long s = 0; //--------------------------
	cin >> n >> k >> m >> l;
	vector<uint> arr (n);
	vector<uint> buff(n);
	vector<int>  count(block);
	vector<int>  pos(block);
	arr[0] = k;
	for (int i = 1; i < n; ++i)
	{
		arr[i] = (uint) ((arr[i - 1] * (unsigned long long) m) & 0xFFFFFFFFU) % l;
	}
	for (int i = 0; i < 32; i += bllen)
	{
		pos[0] = 0;
		for (int j = 0; j < block; ++j)
			count[j] = 0;
		for (int j = 0; j < n; ++j)
			++count[arr[j] >> i & (block - 1)];
		for (int j = 1; j < block; ++j)
			pos[j] = pos[j - 1] + count[j - 1];
		for (int j = 0; j < n; ++j)
			buff[pos[arr[j] >> i & (block - 1)]++] = arr[j];
		arr.swap(buff);
	}
	for (int i = 0; i < n; i += 2)
	{
		s = (s + arr[i]) % l;
	}
	cout << s;           //----------------------------------
	return 0;
}
