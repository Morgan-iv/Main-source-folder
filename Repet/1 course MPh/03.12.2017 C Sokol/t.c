#include<stdio.h>

int main()
{
	int a[64];
	int b[64];
	int i, j;
	for(i = 0; i < 64; ++i)
		scanf("%d", &(a[i]));
	for(i = 0; i < 8; ++i)
		for(j = 0; j < 8; ++j)
		{
			if (j > 0 && i > 1 && a[(i - 2) * 8 + j - 1] == a[i * 8 + j] + 1) b[i * 8 + j] = (i - 2) * 8 + j - 1;
			if (j < 7 && i > 1 && a[(i - 2) * 8 + j + 1] == a[i * 8 + j] + 1) b[i * 8 + j] = (i - 2) * 8 + j + 1;
			if (j < 6 && i > 0 && a[(i - 1) * 8 + j + 2] == a[i * 8 + j] + 1) b[i * 8 + j] = (i - 1) * 8 + j + 2;
			if (j < 6 && i < 7 && a[(i + 1) * 8 + j + 2] == a[i * 8 + j] + 1) b[i * 8 + j] = (i + 1) * 8 + j + 2;
			
			if (j < 7 && i < 6 && a[(i + 2) * 8 + j + 1] == a[i * 8 + j] + 1) b[i * 8 + j] = (i + 2) * 8 + j + 1;
			if (j > 0 && i < 6 && a[(i + 2) * 8 + j - 1] == a[i * 8 + j] + 1) b[i * 8 + j] = (i + 2) * 8 + j - 1;
			if (j > 1 && i < 7 && a[(i + 1) * 8 + j - 2] == a[i * 8 + j] + 1) b[i * 8 + j] = (i + 1) * 8 + j - 2;
			if (j > 1 && i > 0 && a[(i - 1) * 8 + j - 2] == a[i * 8 + j] + 1) b[i * 8 + j] = (i - 1) * 8 + j - 2;
		}
	for(i = 0; i < 64; ++i)
		printf("%d, ", b[i]);
	return 0;
}