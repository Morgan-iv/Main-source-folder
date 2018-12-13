#include<stdio.h>
#include<stdlib.h>

int maxelemdel(double * matrix, int n, int m)
{
	int i, j, s, p;
	s = 0;
	p = 0;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			if (matrix[n * i + j] > matrix[n * s + p])
			{
				s = i;
				p = j;
			}
		}
	}
	for (i = s; i < n - 1; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			matrix[n * i + j] = matrix[n * (i + 1) + j];
		}
	}
	for (j = 0; j < m; ++j)
	{
		matrix[(n - 1) * n + j] = 0;
	}
	return 0;
}

int maxsummdel(double * matrix, int n, int m)
{
	int i, j, s;
	double sum, max;
	max = 0;
	s = 0;
	for (int i = 0; i < m; ++i)
	{
		max += matrix[i];
	}
	for (i = 1; i < n; ++i)
	{
		sum = 0;
		for (j = 0; j < m; ++j)
		{
			sum += matrix[i * n + j];
		}
		if (max < sum)
		{
			max = sum;
			s = i;
		}
	}
	for (i = s; i < n - 1; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			matrix[n * i + j] = matrix[n * (i + 1) + j];
		}
	}
	for (j = 0; j < m; ++j)
	{
		matrix[(n - 1) * n + j] = 0;
	}
	return 0;
}

int mindiagadd(double * matrix, int n, int m)
{
	int i, j, s, p;
	s = 0;
	p = (n < m) ? n : m;
	for (int i = 1; i < p; ++i)
	{
		if (matrix[i * n + i] < matrix[s * n + s]) s = i;
	}
	for (int i = 0; i < n; ++i)
	{
		if (i == s) continue;
		for (int j = 0; j < m; ++j)
		{
			matrix[i * n + j] += matrix[s * n + j];
		}
	}
	return 0;
}

int maxdiagdel(double * matrix, int n, int m)
{
	int i, j, s, p;
	s = 0;
	p = (n < m) ? n : m;
	for (int i = 1; i < p; ++i)
	{
		if (matrix[i * n + i] > matrix[s * n + s]) s = i;
	}
	for (i = s; i < n - 1; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			matrix[n * i + j] = matrix[n * (i + 1) + j];
		}
	}
	for (j = 0; j < m; ++j)
	{
		matrix[(n - 1) * n + j] = 0;
	}
	return 0;
}

int maxelemadd(double * matrix, int n, int m)
{
	int i, j, s, p;
	s = 0;
	p = 0;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			if (matrix[n * i + j] > matrix[n * s + p])
			{
				s = i;
				p = j;
			}
		}
	}
	for (int i = 0; i < n; ++i)
	{
		if (i == s) continue;
		for (int j = 0; j < m; ++j)
		{
			matrix[i * n + j] += matrix[s * n + j];
		}
	}
	return 0;
}

int maxsummadd(double * matrix, int n, int m)
{
	int i, j, s;
	double sum, max;
	max = 0;
	s = 0;
	for (int i = 0; i < m; ++i)
	{
		max += matrix[i];
	}
	for (i = 1; i < n; ++i)
	{
		sum = 0;
		for (j = 0; j < m; ++j)
		{
			sum += matrix[i * n + j];
		}
		if (max < sum)
		{
			max = sum;
			s = i;
		}
	}
	for (int i = 0; i < n; ++i)
	{
		if (i == s) continue;
		for (int j = 0; j < m; ++j)
		{
			matrix[i * n + j] += matrix[s * n + j];
		}
	}
	return 0;
}

int main()
{
	int n, m, i, j;
	double * matrix;
	char c;
	FILE * in, *out;
	in = fopen("input.txt", "r");
	out = fopen("output.txt", "w");
	if (fscanf(in, "%d ", &n) != 1) { fprintf(stderr, "111"); return -1; }
	if (fscanf(in, "%d", &m) != 1) { fprintf(stderr, "222"); return -1; }
	fscanf(in, "%c", &c);
	if (c == '.') { fprintf(stderr, "555"); return -1; }
	if ((n < 0) || (m < 0)) { fprintf(stderr, "333"); return -1; }
	matrix = calloc(n * m, sizeof(double));
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			if (fscanf(in, "%lf", &matrix[n * i + j]) != 1) { fprintf(stderr, "444"); return -1; }
		}
	}
	maxsummadd(matrix, n, m);
	for (i = 0; i < n; ++i)
	{
		fprintf(out, "%.5f", matrix[i * n]);
		for (j = 1; j < m; ++j)
		{
			fprintf(out, " %.5f", matrix[i * n + j]);
		}
		if (i == n - 1) break;
		fprintf(out, "\n");
	}
	return 0;
}
