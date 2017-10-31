#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, l;
	l = EOF;
	while ((c = fgetc(in)) != EOF)
	{
		if (c == l) continue;
		fputc(c, out);
		l = c;
	}
	fclose(in);
	fclose(out);
	return 0;
}