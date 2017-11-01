// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#include<stdio.h>
#include<string.h>

#define M_IN(arg, l, r) ((arg) >= (l) && (arg) <= (r))
#define M_SP(arg) ((arg) == ' ' || (arg) == '\r' || (arg) == '\t' || (arg) == '\n' || (arg) == '\f' || (arg) == '\v') 

typedef unsigned int uint;

int strcmp(const char * str1, const char * str2, int f)
{
	int i, j, f1, f2, n1, n2;
	if (f == 0) return strcmp(str1, str2);
	if (f == 1)
	{
		i = 0;
		while (str1[i] != 0 && str2[i] != 0 && (str1[i] == str2[i] || 
			  (M_IN(str1[i], 'A', 'Z') && M_IN(str2[i], 'a', 'z') && (str1[i] -  'A' == str2[i] -  'a')) ||
			  (M_IN(str1[i], 'a', 'z') && M_IN(str2[i], 'A', 'Z') && (str1[i] -  'a' == str2[i] -  'Z')) ))
			++i;
		return str1[i] - ((int)str2[i]);
	}
	if (f == -1)
	{
		i = 0;
		j = 0;
		f = 0;
		while (str1[i] != 0 && M_SP(str1[i])) ++i;
		while (str2[j] != 0 && M_SP(str2[j])) ++j;
		i += n1 = str1[i] == '-';
		j += n2 = str2[j] == '-';
		while (str1[i] == '0') ++i;
		while (str2[j] == '0') ++j;
		f1 = M_IN(str1[i], '0', '9');
		f2 = M_IN(str2[j], '0', '9');
		if (!(f1 && f2)) return (n1 ? -f1 : f1) - (n2 ? -f2 : f2);
		if (n1 ^ n2) return n2 - n1;
		f1 = 0;
		f2 = 0;
		while (str1[i] != 0 && str2[j] != 0)
		{
			if(M_SP(str1[i]))
			{
				++i;
				continue;
			}
			if(M_SP(str2[j]))
			{
				++j;
				continue;
			}
			if(M_IN(str1[i], '0', '9') && M_IN(str2[j], '0', '9')) (f) ? 0 : (f = (str1[i] - str2[j])), ++f1, ++f2, ++i, ++j;
			else return f1 += M_IN(str1[i], '0', '9'),
						f2 += M_IN(str2[j], '0', '9'),
						((n2 = f1 - f2) ? 0 : n2 = f), 
						(n1 ? -n2 : n2);
		}
		return  f1 += M_IN(str1[i], '0', '9'),
				f2 += M_IN(str2[j], '0', '9'),
				((n2 = f1 - f2) ? 0 : n2 = f), 
				(n1 ? -n2 : n2);
	}
	return 0;
}

int main(int argc, char * argv[])
{
	uint iter, count = 0, i, j, n, k, c;
	FILE * input;
	FILE * output;
	FILE * merge[2];
	char str1[10002];
	char str2[10002];
	char outfilename[2][257];
	int choose, reg, tmp;
	//---------------------------------------------------
	if (argc == 1) return 0;
	outfilename = ".tmpoutsum";
	
	iter = 1;
	while (*(argv[iter]) == '-')
	{
		
	}
	input = fopen("input.txt", "r")
	output = fopen("output1.txt", "w");
	merge[0] = fopen(".tmp1", "w");
	merge[1] = fopen(".tmp2", "w");
	
	while (fgets(str1, 10001, input))
	{
		fputs(str1, merge[count++ & 1]);
	}
	fputc('\n', merge[!(count & 1)]);
	fclose(input);
	fclose(merge[0]);
	fclose(merge[1]);
	merge[0] = fopen(".tmp1", "r");
	merge[1] = fopen(".tmp2", "r");
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
		merge[0] = fopen(".tmp1", "w");
		merge[1] = fopen(".tmp2", "w");
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
		merge[0] = fopen(".tmp1", "r");
		merge[1] = fopen(".tmp2", "r");
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