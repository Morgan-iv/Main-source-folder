#include<stdio.h>
#include<stdlib.h>

int strmaxsumvych(double * arr, int a, int b)
{
	int i, j, str;
	double sum, max;
	max = 0;
	str = 0;
	for (int i = 0; i < b; ++i)
	{
		max += arr[i];
	}
	for (i = 1; i < a; ++i)
	{
		sum = 0;
		for (j = 0; j < b; ++j)
		{
			sum += arr[i * a + j];
		}
		if (max < sum)
		{
			max = sum;
			str = i;
		}
	}
	for (int i = 0; i < a; ++i)
	{
		if (i == str) continue;
		for (int j = 0; j < b; ++j)
		{
			arr[i * a + j] -= arr[str * a + j];
		}
	}
	return 0;
}

int main()
{
	int a, b, i, j;
	FILE * input, *output;
	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");
	if (fscanf(input, "%d ", &a) != 1) return -1;
	if (fscanf(input, "%d", &b) != 1) return -1;
	char e;
	fscanf(input, "%c", &e);
	if (e == '.') return -1;
	double * arr;
	if (a < 0 || b < 0) return -1;
	arr = malloc(a * b * sizeof(double));
	for (i = 0; i < a; ++i)
	{
		for (j = 0; j < b; ++j)
		{
			if (fscanf(input, "%lf", &arr[a * i + j]) != 1) return -1;
		}
	}
	strmaxsumvych(arr, a, b);
	for (i = 0; i < a; ++i)
	{
		fprintf(output, "%.5f", arr[i * a]);
		for (j = 1; j < b; ++j)
		{
			fprintf(output, " %.5f", arr[i * a + j]);
		}
		if (i == a - 1) break;
		fprintf(output, "\n");
	}
	return 0;
}

int strmaxudal(double * arr, int a, int b)
{
	int i, j, str, stolb;
	str = 0;
	stolb = 0;
	for (i = 0; i < a; ++i)
	{
		for (j = 0; j < b; ++j)
		{
			if (arr[a * i + j] > arr[a * str + stolb])
			{
				str = i;
				stolb = j;
			}
		}
	}
	for (i = str; i > 0; --i)
	{
		for (j = 0; j < b; ++j)
		{
			arr[a * i + j] = arr[a * (i - 1) + j];
		}
	}
	for (j = 0; j < b; ++j)
	{
		arr[j] = 0;
	}
	return 0;
}

int strmaxsumudal(double * arr, int a, int b)
{
	int i, j, str;
	double sum, max;
	max = 0;
	str = 0;
	for (int i = 0; i < b; ++i)
	{
		max += arr[i];
	}
	for (i = 1; i < a; ++i)
	{
		sum = 0;
		for (j = 0; j < b; ++j)
		{
			sum += arr[i * a + j];
		}
		if (max < sum)
		{
			max = sum;
			str = i;
		}
	}
	for (i = str; i > 0; --i)
	{
		for (j = 0; j < b; ++j)
		{
			arr[a * i + j] = arr[a * (i - 1) + j];
		}
	}
	for (j = 0; j < b; ++j)
	{
		arr[j] = 0;
	}
	return 0;
}

int diagminvych(double * arr, int a, int b)
{
	int i, j, str, diagmax;
	str = 0;
	diagmax = (a < b) ? a : b;
	for (int i = 1; i < diagmax; ++i)
	{
		if (arr[i * a + i] < arr[str * a + str]) str = i;
	}
	for (int i = 0; i < a; ++i)
	{
		if (i == str) continue;
		for (int j = 0; j < b; ++j)
		{
			arr[i * a + j] -= arr[str * a + j];
		}
	}
	return 0;
}

int diagmaxudal(double * arr, int a, int b)
{
	int i, j, str, diagmax;
	str = 0;
	diagmax = (a < b) ? a : b;
	for (int i = 1; i < diagmax; ++i)
	{
		if (arr[i * a + i] > arr[str * a + str]) str = i;
	}
	for (i = str; i > 0; --i)
	{
		for (j = 0; j < b; ++j)
		{
			arr[a * i + j] = arr[a * (i - 1) + j];
		}
	}
	for (j = 0; j < b; ++j)
	{
		arr[j] = 0;
	}
	return 0;
}

int strmaxvych(double * arr, int a, int b)
{
	int i, j, str, stolb;
	str = 0;
	stolb = 0;
	for (i = 0; i < a; ++i)
	{
		for (j = 0; j < b; ++j)
		{
			if (arr[a * i + j] > arr[a * str + stolb])
			{
				str = i;
				stolb = j;
			}
		}
	}
	for (int i = 0; i < a; ++i)
	{
		if (i == str) continue;
		for (int j = 0; j < b; ++j)
		{
			arr[i * a + j] -= arr[str * a + j];
		}
	}
	return 0;
}

int strmaxsumvych(double * arr, int a, int b)
{
	int i, j, str;
	double sum, max;
	max = 0;
	str = 0;
	for (int i = 0; i < b; ++i)
	{
		max += arr[i];
	}
	for (i = 1; i < a; ++i)
	{
		sum = 0;
		for (j = 0; j < b; ++j)
		{
			sum += arr[i * a + j];
		}
		if (max < sum)
		{
			max = sum;
			str = i;
		}
	}
	for (int i = 0; i < a; ++i)
	{
		if (i == str) continue;
		for (int j = 0; j < b; ++j)
		{
			arr[i * a + j] -= arr[str * a + j];
		}
	}
	return 0;
}
