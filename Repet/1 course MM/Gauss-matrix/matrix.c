
void swap (double * row1, double * row2, int n)
{
	double tmp;
	int i;
	for (i = 0; i < n; ++i)
	{
		tmp = row1[i];
		row1[i] = row2[i];
		row2[i] = tmp;
	}
}

void appl (double * row1, double * row2, double a, int n)
{
	int i;
	for (i = 0; i < n; ++i)
	{
		row1[i] += row2[i] * a;
	}
}

void mul (double * row, double a, int n)
{
	int i;
	for (i = 0; i < n; ++i)
	{
		row[i] *= a;
	}
}

int reverse (double * matrix, double * res, int n)
{
	int tmp, i, j;
	tmp = 0;
	double k;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			res[i * n + j] = (i != j) ? 0.0 : 1.0;
		}
	}
	for (i = 0; i < n; ++i)
	{
		if (matrix[i * (n + 1)] == 0.0)
		{
			tmp = 0;
			for (j = i + 1; j < n; ++j)
			{
				if (matrix[j * n + i] != 0)
				{
					swap(matrix + (i * n), matrix + (j * n), n);
					swap(res + (i * n), res + (j * n), n);
					tmp = 1;
					break;
				}
			}
			if (!tmp)
			{
				return 1;
			}
		}
		k = 1.0 / matrix[i * (n + 1)];
		mul(matrix + (i * n), k, n);
		mul(res + (i * n), k, n);
		for (j = i + 1; j < n; ++j)
		{
			k = -matrix[j * n + i];
			appl(matrix + (j * n), matrix + (i * n), k, n);
			appl(res + (j * n), res + (i * n), k, n);
		}
	}
	for (i = n - 1; i > 0; --i)
	{
		for (j = i - 1; j >= 0; --j)
		{
			k = -matrix[j * n + i];
			appl(matrix + (j * n), matrix + (i * n), k, n);
			appl(res + (j * n), res + (i * n), k, n);
		}
	}
	return 0;
}