#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char sub[4005], str[4005];
	fgets(sub, 4002, in);
	for (i = 0; sub[i] != '\0'; ++i)
		1;
	if (!i)
	{
		fclose(in);
		fclose(out);
		return 0;
	}
	sub[i - 1] = '\0';
	n = 0;
	str[0] = '\0';
	while ((c = fgetc(in)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		{
			if (!strstr(str, sub))
				fputs(str, out);
			str[0] = '\0';
			n = 0;
			fputc(c, out);
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
