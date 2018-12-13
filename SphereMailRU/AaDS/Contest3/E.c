// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<stdio.h>
#include<string.h>

typedef unsigned int uint;

int main()
{
	uint iter, count = 0, i, j, n, k, c;
	FILE * input = fopen("input.txt", "r");
	FILE * output = fopen("output1.txt", "w");
	FILE * merge[2];
	merge[0] = fopen("in1.txt", "w");
	merge[1] = fopen("in2.txt", "w");
	char str1[10002];
	char str2[10002];
	int choose;
	while (fgets(str1, 10001, input))
	{
		fputs(str1, merge[count++ & 1]);
	}
	fputc('\n', merge[!(count & 1)]);
	fclose(input);
	fclose(merge[0]);
	fclose(merge[1]);
	merge[0] = fopen("in1.txt", "r");
	merge[1] = fopen("in2.txt", "r");
	for (iter = 0; (count - 1) >> iter > 0; ++iter)
	{
		n = (1 << iter);
		k = count >> (iter + 1);
		for (c = 0; c < k; ++c)
		{
			for (i = 0, j = 0,
				fgets(str1, 10001, merge[0]),
				fgets(str2, 10001, merge[1]);
				i < n && j < n;)
			{
				choose = strcmp(str1, str2);
				if (choose < 0)
				{
					fputs(str1, output);
					++i;
					if (i < n) fgets(str1, 10001, merge[0]);
				}
				else
				{
					fputs(str2, output);
					++j;
					if (j < n) fgets(str2, 10001, merge[1]);
				}
			}
			if (i < n)
			{
				fputs(str1, output);
				for (++i; i < n; ++i)
				{
					fgets(str1, 10001, merge[0]);
					fputs(str1, output);
				}
			}
			if (j < n)
			{
				fputs(str2, output);
				for (++j; j < n; ++j)
				{
					fgets(str2, 10001, merge[1]);
					fputs(str2, output);
				}
			}
		}
		k = count & ((1 << iter) - 1);
		c = count & (1 << iter);
		if (!(c && k))
		{
			c += k;
			for (i = 0; i < c; ++i)
			{
				fgets(str1, 10001, merge[0]);
				fputs(str1, output);
			}
		}
		else
		{
			for (i = 0, j = 0,
				fgets(str1, 10001, merge[0]),
				fgets(str2, 10001, merge[1]);
				i < n && j < k;)
			{
				choose = strcmp(str1, str2);
				if (choose < 0)
				{
					fputs(str1, output);
					++i;
					if (i < n) fgets(str1, 10001, merge[0]);
				}
				else
				{
					fputs(str2, output);
					++j;
					if (j < k) fgets(str2, 10001, merge[1]);
				}
			}
			if (i < n)
			{
				fputs(str1, output);
				for (++i; i < n; ++i)
				{
					fgets(str1, 10001, merge[0]);
					fputs(str1, output);
				}
			}
			if (j < k)
			{
				fputs(str2, output);
				for (++j; j < k; ++j)
				{
					fgets(str2, 10001, merge[1]);
					fputs(str2, output);
				}
			}
		}
		fclose(output);
		fclose(merge[0]);
		fclose(merge[1]);
		if ((count - 1) >> iter == 1)
			break;
		output = fopen("output1.txt", "r");
		merge[0] = fopen("in1.txt", "w");
		merge[1] = fopen("in2.txt", "w");
		k = ((count - 1) / (n <<= 1)) + 1;
		for (c = 0; c < k; ++c)
		{
			i = 0;
			while (i++ < n && fgets(str1, 10001, output))
			{
				fputs(str1, merge[c & 1]);
			}
		}
		fclose(output);
		fclose(merge[0]);
		fclose(merge[1]);
		output = fopen("output1.txt", "w");
		merge[0] = fopen("in1.txt", "r");
		merge[1] = fopen("in2.txt", "r");
	}
	input = fopen("output1.txt", "r");
	output = fopen("output.txt", "w");
	for (i = 1; i < count; ++i)
	{
		fgets(str1, 10001, input);
		fputs(str1, output);
	}
	fgets(str1, 10001, input);
	str1[strlen(str1) - 1] = '\0';
	fputs(str1, output);
	fclose(output);
	fclose(input);
	return 0;
}
