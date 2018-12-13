
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int Z13(const char * ifile, const char * ofile)
{
	int * arr;
	int x, k;
	int p = 0, size = 10;
	double d;
	FILE * f;
	f = fopen(ifile, "r");
	arr = (int *) malloc(sizeof(int) * size);
	while ((k = fscanf(f, " %d ", &x)) != EOF)
	{
		if (p == size)
		{
			arr = (int *) realloc(arr, (size *= 2) * sizeof(int));
		}
		arr[p++] = x;
	}
	fclose(f);
	x = 0;
	size = p;
	f = fopen(ofile, "w");
	for (p = 0; p < size; ++p)
	{
		x = 1;
		d = sqrt(arr[p]);
		for (k = 2; k <= d; ++k)
			x = x && arr[p] % k;
		if (x)
			fprintf(f, "%d ", arr[p]);
	}
	free(arr);
	fclose(f);
	return 0;
}
