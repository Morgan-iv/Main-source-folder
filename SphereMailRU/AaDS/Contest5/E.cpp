// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<stdio.h>
#include<stdlib.h>

typedef unsigned int uint;

void prop(int * arr, uint i)
{
	if (i == 1) return;
	arr[i >> 1] = arr[i] ^ arr[i ^ 1];
	prop(arr, i >> 1);
}

int op_xor(int * arr, uint left, uint right)
{
	int res = 0;
	if (left & 1) res ^= arr[left++];
	if (!(right & 1)) res ^= arr[right--];
	if (right > left) res ^= op_xor (arr, left >> 1, right >> 1);
	return res;
}

int main()
{
	uint n, m, code, l, r, c;
	int * arr;
	scanf("%d %d", &n, &m);
	for (code = 1; (n - 1) >> code > 0; ++code);
	c = (1 << code);
	arr = (int *)calloc(c << 1, sizeof(int));
	for (int i = c; i < n + c; ++i)
	{
		scanf("%d", arr + i);
		prop(arr, i);
	}
	for (int i = 0; i < m; ++i)
	{
		scanf("%d %d %d", &code, &l, &r);
		if (code == 2)
		{
			arr[c + l] = r;
			prop(arr, c + l);
		}
		else
		{
			printf("%d\n", op_xor (arr, c + l, c + r));
		}
	}
	return 0;
}
