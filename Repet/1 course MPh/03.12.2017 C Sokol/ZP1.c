#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int sort1n(double * arr, int n)
{
	int i, j;
	double tmp;
	for (i = 0; i < n - 1; ++i)
		for (j = 0; j < n - i - 1; ++j)
			if (arr[j] > arr[j + 1])
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
	return 0;
}

int sort1f(char ** arr, int n)
{
	int i, j;
	char * tmp;
	for (i = 0; i < n - 1; ++i)
		for (j = 0; j < n - i - 1; ++j)
			if (strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
	return 0;
}

int sort2n(double * arr, int n)
{
	int j;
	double tmp;
	j = 0;
	while (j < n - 1)
	{
		if (arr[j] > arr[j + 1])
		{
			tmp = arr[j];
			arr[j] = arr[j + 1];
			arr[j + 1] = tmp;
			if (j > 0) --j;
			else ++j;
		}
		else ++j;
	}
	return 0;
}

int sort2f(char ** arr, int n)
{
	int i, j;
	char * tmp;
	j = 0;
	while (j < n - 1)
	{
		if (strcmp(arr[j], arr[j + 1]) > 0)
		{
			tmp = arr[j];
			arr[j] = arr[j + 1];
			arr[j + 1] = tmp;
			if (j > 0) --j;
			else ++j;
		}
		else ++j;
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	char * sarr[101];
	double arr1[20];
	double arr2[20];
	FILE * in;
	FILE * out;
	int i, n;
	in = fopen("input.txt", "r");
	out = fopen("output.txt", "w");
	sarr[0] = malloc(82 * sizeof(char));
	i = 0;
	while (fgets(sarr[i++], 82, in))
		sarr[i] = malloc(82 * sizeof(char));
	n = i - 1;
	sort1f(sarr, n);
	for (i = 0; i < n; ++i)
		fprintf(out, "%s", sarr[i]);
	
	for (i = 0; i < 20; ++i)
	{
		arr1[i] = arr2[i] = (rand() % 30);
	}
	sort1n(arr2, 20);
	for (i = 0; i < 20; ++i)
	{
		printf("%f %f\n", arr1[i], arr2[i]);
	}
	return 0;
}