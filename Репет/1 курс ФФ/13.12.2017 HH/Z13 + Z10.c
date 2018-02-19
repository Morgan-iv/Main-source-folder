// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int Z10(const char * ifile1, const char * ifile2, const char * ofile, char op)
{
	double * arr1;
	double * arr2;
	double x;
	int p = 0, size = 10;
	FILE * f;
	f = fopen(ifile1, "r");
	arr1 = (double *) malloc(sizeof(double) * size);
	while (fscanf(f, " %lf ", &x) != EOF)
	{
		if (p == size)
		{
			arr1 = (double *) realloc(arr1, (size *= 2) * sizeof(double));
		}
		arr1[p++] = x;
	}
	fclose(f);
	f = fopen(ifile2, "r");
	size = p;
	p = 0;
	arr2 = (double *) malloc(sizeof(double) * size);
	while (fscanf(f, " %lf ", &x) != EOF)
		arr2[p++] = x;
	fclose(f);
	x = 0;
	f = fopen(ofile, "w");
	for (p = 0; p < size; ++p)
	{
		if (op == '+')
			fprintf(f, "%f ", arr1[p] + arr2[p]);
		else
			x += arr1[p] * arr2[p];
	}
	if (op == '*')
		fprintf(f, "%f", x);
	free(arr1);
	free(arr2);
	fclose(f);
	return 0;
}

int Z13(const char * ifile, const char * ofile)
{
	int * arr;
	int x, k;
	int p = 0, size = 10;
	FILE * f;
	f = fopen(ifile, "r");
	arr = (int *) malloc(sizeof(int) * size);
	while ((k = fscanf(f, " %d ", &x)) != EOF)
	{
		if (k == 1)
		{
			if (p == size)
			{
				arr = (int *) realloc(arr, (size *= 2) * sizeof(int));
			}
			arr[p++] = x;
		}
		else
			fscanf(f, "%c", &x);
	}
	fclose(f);
	x = 0;
	size = p;
	f = fopen(ofile, "w");
	for (p = 0; p < size; ++p)
	{
		fprintf(f, "%d ", arr[p]);
		x += arr[p];
	}
	fprintf(f, "summa=%d", x);
	free(arr);
	fclose(f);
	return 0;
}

int main()
{
	printf("%d", Z13("input.txt", "out.txt"));
	printf("%d", Z10("in1.txt", "in2.txt", "out.txt", '+'));
	return 0;
}