#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char sub[4005];
	sub[0] = '\0';
	i = 1;
	n = 0;
	while ((c = fgetc(in)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		{
			if (sub[0] != '\0')
			{
				fputs(sub, out);
				fputc('\n', out);
				sub[0] = '\0';
			}
			i = 1;
			n = 0;
			continue;
		}
		if (i && c >= '0' && c <= '9')
		{
			sub[n] = c;
			++n;
			sub[n] = '\0';
			continue;
		}
		i = 0;
		sub[0] = '\0';
	}
	if (sub[0] != '\0')
		fputs(sub, out);
	fclose(in);
	fclose(out);
	return 0;
}