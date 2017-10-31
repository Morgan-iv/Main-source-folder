#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char sub[4005], str[4005];
	for (i = 0; (c = fgetc(in)) != '\n' && c != EOF; ++i)
		sub[i] = c;
	sub[i] = '\0';
	n = 0;
	i = 2;
	fprintf(out, "1\n");
	str[0] = '\0';
	while ((c = fgetc(in)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		{
			if (!strcmp(str, sub))
				fprintf(out, "%d\n", i);
			str[0] = '\0';
			n = 0;
			if (c == '\n') ++i;
			continue;
		}
		str[n] = c;
		++n;
		str[n] = '\0';
	}
	fclose(in);
	fclose(out);
	return 0;
}