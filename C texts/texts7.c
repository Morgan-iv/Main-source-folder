#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i, n;
	char str[4005];
	char sub[4005];
	sub[0] = '\0';
	while (fgets(str, 4004, in))
	{
		for(n = 0; str[n] == ' '; ++n)
			1;
		for(i = n; str[i] != '\0'; ++i)
			1;
		if (str[i - 2] != '-')
		{
			fputs(sub, out);
			sub[0] = '\0';
			fputs(&(str[n]), out);
			continue;
		}	
		str[i - 2] = '\0';
		for(; str[i] != ' '; --i)
			1;
		++i;
		fputs(sub, out);
		strcpy(sub, &(str[i]));
		str[i - 1] = '\n';
		str[i] = '\0';
		fputs(&(str[n]), out);
	}
	if (sub[0] != '\0')
	{
		fputs(sub, out);
		fputc('-', out);
	}
	fclose(in);
	fclose(out);
	return 0;
}