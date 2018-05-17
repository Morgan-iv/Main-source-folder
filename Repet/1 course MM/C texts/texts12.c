#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char subb[4005], sube[4005], str[4005];
	for (i = 0; (c = fgetc(in)) != ' ' && c != EOF; ++i)
		subb[i] = c;
	subb[i] = '\0';
	for (i = 0; (c = fgetc(in)) != '\n' && c != EOF; ++i)
		sube[i] = c;
	sube[i] = '\0';
	n = 0;
	str[0] = '\0';
	while ((c = fgetc(in)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		{
			if (n >= i && strstr(str, subb) == str && !strcmp(&(str[n - i]), sube))
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