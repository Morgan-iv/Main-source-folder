#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void printm (double * matrix, int n, FILE * f)
{
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			fprintf(f, "%6.3f ", matrix[i * n + j]);
		}
		fprintf(f, "\n");
	}
}

void scanm (double * matrix, int n, FILE * f)
{
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			fscanf(f, "%lf", &(matrix[i * n + j]));
		}
	}
}

int main()
{
	int i, j, n, isbad, itype;
	double * mat;
	double * res;
	char filename[20];
	printf("Please enter type of input\n");
	printf("1: console\n");
	printf("2: file\n");
	scanf("%d", &itype);
	if (itype == 1)
	{
		
		printf("Please input size of matrix\n");
		scanf("%d", &n);
		mat = malloc(n *n * sizeof(double *));
		res = malloc(n *n * sizeof(double *));
		if (!(mat && res))
		{
			printf("Error1\n");
			return 1;
		}

/*		for (i = 0; i < n; ++i)
		{
			mat[i] = malloc(n * sizeof(double));
			res[i] = malloc(n * sizeof(double));
			if (!(mat[i] && res[i]))
			{
				printf("Error2\n");
				return 1;
			}	
		}*/
		printf("Please input matrix\n");
		scanm(mat, n, stdin);

	}
	else if (itype == 2)
	{
		n = 20;
		FILE * file;
		printf("Please input filename\n");
		scanf("%s", filename);
		file = fopen(filename, "r");
		if (!file)
		{
			printf("No such file\n");
			return 1;
		}
		fscanf(file, "%d", &n);
		mat = malloc(n * n * sizeof(double));
		res = malloc(n * n * sizeof(double));
		if (!(mat && res))
		{
			printf("Error1\n");
			return 1;
		}
		/*
		for (i = 0; i < n; ++i)
		{
			mat[i] = malloc(n * sizeof(double));
			res[i] = malloc(n * sizeof(double));
			if (!(mat[i] && res[i]))
			{
				printf("Error2\n");
				return 1;
			}	
		}
		*/
		scanm(mat, n, file);
	}
	else
	{
		printf("Wrong option\n");
		return 1;
	}
	isbad = reverse(mat, res, n);
	if (isbad)
	{
		printf("Bad Determinant\n");
		return 1;
	}
	/*
	printf("\n");
	printm(mat, n, stdout);
	*/
	printf("\n");
	printm(res, n, stdout);
	return 0;
}
