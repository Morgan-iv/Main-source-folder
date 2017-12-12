#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, j, n;
	char str[4005];
	while (fgets(str, 4004, in))
	{
		c = 1;
		j = 0;
		n = 0;
		for(i = 0; str[i] != '\0'; ++i)
		{
			c = c && str[i] == ' ';
			if (str[i] == '\n')
			{
				n = 1;
				continue;
			}
			if (str[i] != ' ')
			{
				if (j) fputc('-', out);
				j = 0;
				fputc(str[i], out);
			}
			if (c && str[i] == ' ')
				fputc(' ', out);
			if (!c && str[i] == ' ')
				++j;
		}
		for(i = 0; i < j; ++i)
			fputc(' ', out);
		if (n) fputc('\n', out);
	}
	fclose(in);
	fclose(out);
	return 0;
}