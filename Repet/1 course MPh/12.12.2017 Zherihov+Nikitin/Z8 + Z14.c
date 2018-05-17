// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int Z14(const char * filename)
{
	char * symbols;
	char za4et[5];
	int c, p = 0, size = 10;
	FILE * f;
	za4et[0] = za4et[1] = za4et[2] = za4et[3] = za4et[4] = 0;
	f = fopen(filename, "r");
	symbols = (char *) malloc(sizeof(char) * size);
	while (fscanf(f, " %c ", &c) != EOF)
	{
		if (p == size)
		{
			symbols = (char *) realloc(symbols, (size *= 2) * sizeof(char));
		}
		symbols[p++] = c;
	}
	for (c = 0; c < p; ++c)
	{
		if (symbols[c] == 'z') za4et[0] = 1;
		if (symbols[c] == 'a') za4et[1] = 1;
		if (symbols[c] == '4') za4et[2] = 1;
		if (symbols[c] == 'e') za4et[3] = 1;
		if (symbols[c] == 't') za4et[4] = 1;
	}
	free(symbols);
	fclose(f);
	return za4et[0] && za4et[1] && za4et[2] && za4et[3] && za4et[4];
}

int Z8(const char * ifile, const char * ofile)
{
	double * arr;
	double x;
	int p = 0, size = 10;
	FILE * f;
	f = fopen(ifile, "r");
	arr = (double *) malloc(sizeof(double) * size);
	while (fscanf(f, " %lf ", &x) != EOF)
	{
		if (p == size)
		{
			arr = (double *) realloc(arr, (size *= 2) * sizeof(double));
		}
		arr[p++] = x;
	}
	fclose(f);
	f = fopen(ofile, "w");
	size = p;
	for (p = 0; p < size; ++p)
	{
		fprintf(f, "%f %f\n", arr[p], x *= (x = cos(arr[p])));
	}
	free(arr);
	fclose(f);
	return 0;
}

int main()
{
	printf("%d", Z8("input.txt", "out.txt"));
}