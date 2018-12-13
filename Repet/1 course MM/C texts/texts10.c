#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n, f;
	char sub[4005], sym[256];
	for(i = 0; i < 256; ++i) 
		sym[i] = 0;
	sub[0] = '\0';
	f = 0;
	n = 0;
	while ((c = fgetc(in)) != '\n' && c != EOF)
		sym[c] = 1;
	while ((c = fgetc(in)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		{
			if (f)
			{
				fputs(sub, out);
				fputc('\n', out);
				sub[0] = '\0';
				f = 0;
			}
			n = 0;
			continue;
		}
		sub[n] = c;
		f = f || (!n && sym[c]);
		++n;
		sub[n] = '\0';
	}
	if (f)
		fputs(sub, out);
	fclose(in);
	fclose(out);
	return 0;
}
