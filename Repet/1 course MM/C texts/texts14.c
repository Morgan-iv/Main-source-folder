#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char str[256], sym[256];
	for (i = 0; i < 256; ++i) 
		sym[i] = i;
	for (i = 0; (c = fgetc(in)) != ' ' && c != EOF; ++i)
		str[i] = c;
	for (i = 0; (c = fgetc(in)) != '\n' && c != EOF; ++i)
		sym[str[i]] = c;
	while ((c = fgetc(in)) != EOF)
		fputc(sym[c], out);
	fclose(in);
	fclose(out);
	return 0;
}
