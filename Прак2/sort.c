// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end
#define WINDEBUG

#include<stdio.h>
#include<string.h>
#ifndef WINDEBUG
#include<unistd.h>
#endif

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

char * mfgets(char ** str, uint * count, FILE * stream)
{
	int ptr = 0;
	*(*str) = 0;
	while (fgets((*str) + ptr, *count + 1, stream) && strlen((*str) + ptr) == *count - ptr && (*str)[*count - 1] != '\n')
		ptr = *count, *count <<= 1, (*str) = realloc((*str), *count + 1);
	if (!(*(*str))) return NULL;
	return (*str);
}

int sortfile(const char * inp, const char * out, const char * tmp1, const char * tmp2, int f)
{
	uint iter, count = 0, i, j, n, k, c, len;
	FILE * input, * output;
	FILE * merge[2];
	char * str1, * str2;
	int choose;
	//---------------------------------------------------
	
	
	len = 1000;
	str1 = malloc(len + 1);
	input =  fopen(inp, "r")
	output = fopen(out, "w");
	merge[0] = fopen(tmp1, "w");
	merge[1] = fopen(tmp2, "w");
	
	
	while (mfgets(&str1, &len, input))
	{
		fputs(str1, merge[count++ & 1]);
	}
	fputc('\n', merge[!(count & 1)]);
	
	
	fclose(input);
	fclose(merge[0]);
	fclose(merge[1]);
	merge[0] = fopen(tmp1, "r");
	merge[1] = fopen(tmp2, "r");
	str2 = malloc(len + 1);
	
	
	for (iter = 0; (count - 1) >> iter > 0; ++iter)
	{
		n = (1 << iter);
		k = count >> (iter + 1);
		for (c = 0; c < k; ++c)
		{
			for (i = 0, j = 0,
				fgets(str1, len, merge[0]),
				fgets(str2, len, merge[1]);
				i < n && j < n;)
			{
				choose = strcmp(str1, str2, f);
				if (choose < 0)
				{
					fputs(str1, output);
					++i;
					if (i < n) fgets(str1, len, merge[0]);
				}
				else
				{
					fputs(str2, output);
					++j;
					if (j < n) fgets(str2, len, merge[1]);
				}
			}
			if (i < n)
			{
				fputs(str1, output);
				for (++i; i < n; ++i)
				{
					fgets(str1, len, merge[0]);
					fputs(str1, output);
				}
			}
			if (j < n)
			{
				fputs(str2, output);
				for (++j; j < n; ++j)
				{
					fgets(str2, len, merge[1]);
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
				fgets(str1, len, merge[0]);
				fputs(str1, output);
			}
		}
		
		
		else
		{
			for (i = 0, j = 0,
				fgets(str1, len, merge[0]),
				fgets(str2, len, merge[1]);
				i < n && j < k;)
			{
				choose = strcmp(str1, str2, f);
				if (choose < 0)
				{
					fputs(str1, output);
					++i;
					if (i < n) fgets(str1, len, merge[0]);
				}
				else
				{
					fputs(str2, output);
					++j;
					if (j < k) fgets(str2, len, merge[1]);
				}
			}
			if (i < n)
			{
				fputs(str1, output);
				for (++i; i < n; ++i)
				{
					fgets(str1, len, merge[0]);
					fputs(str1, output);
				}
			}
			if (j < k)
			{
				fputs(str2, output);
				for (++j; j < k; ++j)
				{
					fgets(str2, len, merge[1]);
					fputs(str2, output);
				}
			}
		}
		
		
		fclose(output);
		fclose(merge[0]);
		fclose(merge[1]);
		if ((count - 1) >> iter == 1)
			break;
		
		
		output = fopen(out, "r");
		merge[0] = fopen(tmp1, "w");
		merge[1] = fopen(tmp2, "w");
		k = ((count - 1) / (n <<= 1)) + 1;
		
		
		for (c = 0; c < k; ++c)
		{
			i = 0;
			while (i++ < n && fgets(str1, len, output))
			{
				fputs(str1, merge[c & 1]);
			}
		}
		
		
		fclose(output);
		fclose(merge[0]);
		fclose(merge[1]);
		output = fopen(out, "w");
		merge[0] = fopen(tmp1, "r");
		merge[1] = fopen(tmp2, "r");
	}
	free(str1);
	free(str2);
	return (int)count;
}

int mergefiles(const char * in1, const char * in2, const char * out, int f, int bn)
{
	uint len1, len2;
	char * str1, * str2, * tmp1, * tmp2;
	FILE * input1, * input2, * output;
	int choose;
	
	len1 = len2 = 1000;
	str1 = malloc(len1 + 1);
	str2 = malloc(len2 + 1);
	input1 = fopen(in1, "r");
	input2 = fopen(in2, "r");
	output = fopen(out, "w");
	tmp1 = mfgets(&str1, &len1, input1);
	tmp2 = mfgets(&str2, &len2, input2);
	
	
	while (tmp1 && tmp2)
	{
		choose = strcmp(str1, str2, f);
		if (choose < 0)
		{
			fputs(str1, output);
			tmp1 = mfgets(&str1, &len1, input1);
		}
		else
		{
			fputs(str2, output);
			tmp2 = mfgets(&str2, &len2, input2);
		}
	}
	
	if (!tmp1)
	{
		if (bn && tmp2) str2[strlen(str2) - 1] = 0;
		if (tmp2) fputs(str2, output);
		while(mfgets(&str2, &len2, input2))
		{
			if (bn)
			{
				str2[strlen(str2) - 1] = 0;
				fputc('\n', output)
			}
			fputs(str2, output);
		}
		tmp2 = NULL;
	}
		
	if (!tmp2)
	{
		if (bn && tmp1) str1[strlen(str1) - 1] = 0;
		if (tmp1) fputs(str1, output);
		while(mfgets(&str1, &len1, input1))
		{
			if (bn)
			{
				str1[strlen(str1) - 1] = 0;
				fputc('\n', output)
			}
			fputs(str1, output);
		}
		tmp1 = NULL;
	}
	free(str1);
	free(str2);
	fclose(input1);
	fclose(input2);
	fclose(output);
	return 0;
}

int check (const char * filename)
{
	uint len1, len2, t;
	FILE * file;
	char * str1, * str2, * tmp;
	int res;
	
	len1 = len2 = 1000;
	str1 = malloc(len1 + 1);
	str2 = malloc(len2 + 1);
	file = fopen(filename, "r");
	
	tmp = mfgets(&str1, &len1, file);
	if (!tmp) return 0;
	
	while(mfgets(&str2, &len2, file))
	{
		if(strcmp(str1, str2) > 0) return 1;
		tmp = str1, str1 = str2, str2 = tmp;
		t = len1, len1 = len2, len2 = t;
	}
	
	return 0;
}

int main(int argc, char * argv[])
{
	int iter, f, i, e, j, o;
	char files[4][257];
	char output[257];
	FILE * tmp;
	//---------------------------------------------
	if (argc == 1) return 0;
	*output = 0;
	iter = 1;
	f = 0;
	while(*(argv[iter]) == '-')
	{
		if (argv[iter][1] == 'c') f = 2;
		if (argv[iter][1] == 'n') f = 1;
		if (argv[iter][1] == 'f') f = (f) ? f : -1;
		if (argv[iter][1] == 'o') strcpy(output, argv[++iter]), o = 1;
		++iter;
	}
	
	
	if (f == 2)
	{
		tmp = fopen(argv[iter], r)
		if (!tmp)
		{
			printf("Check failed");
			return 2;
		}
		fclose(tmp);
		printf("Check file %s - result is %d", argv[iter], e = check(argv[iter]));
		return e;
	}
	
	
	j = 1;
	files[0][0] = '.';
	files[0][1] = 'a';
	files[0][2] = 0;
	for(i = 0; i < 4; ++i)
	{
		if (i) strcpy(files[i], files[i - 1]);
		e = 1;
		while(e)
		{
			if((files[i][j])++ > 'z')
			{
				if (j == 255)
				{
					printf("WUTFACE 26 * 255 files here, cant work in this directory");
					return 1;
				}
				for(e = 1; e <= j; ++e)
					files[i][e] = 'a';
				files[i][++j] = 'a';
				files[i][j + 1] = 0;
			}
			#ifdef  WINDEBUG
			e = 0;
			#else
			e = access(files[i], F_OK);
			#endif
		}
	}
	
	
	tmp = fopen(files[0], "w");
	fclose(tmp);
	tmp = fopen(files[1], "w");
	fclose(tmp);
	tmp = fopen(files[2], "w");
	fclose(tmp);
	tmp = fopen(files[3], "w");
	fclose(tmp);
	
	
	e = 0;
	for(i = iter; i < argc; ++i)
	{
		tmp = fopen(argv[i], "r");
		if (!tmp)
		{
			if (o)
			{
				tmp = fopen(files[2], "w");
				fclose(tmp);
				mergefiles(files[e], files[2], output, f, 1);
			}
			else
			{
				tmp = fopen(files[e]);
				while((f = fgetc(tmp)) != EOF) putchar(f);
				fclose(tmp);
			}
			printf("File number %d does not exist", i - iter + 1);
			return 1;
		}
		fclose(tmp);
		sortfile(argv[i], files[3], files[2], files[!e], f);
		mergefiles(files[3], files[e], files[!e], f, 0);
		e = !e;
	}
	if (o)
	{
		tmp = fopen(files[2], "w");
		fclose(tmp);
		mergefiles(files[e], files[2], output, f, 1);
	}
	else
	{
		tmp = fopen(files[e]);
		while((f = fgetc(tmp)) != EOF) putchar(f);
		fclose(tmp);
	}
	printf("%d files sorted successfully", i - iter + 1);
	return 0;
}
