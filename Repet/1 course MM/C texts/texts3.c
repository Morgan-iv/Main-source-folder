#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i;
	char str[4005];
	while (fgets(str, 4004, in))
	{
		if (str[0] == '\0' || str[0] == '\n') continue;
		fprintf(out, "%c%c", '\n', str[0]);
		i = 0;
		while (str[i + 1] != '\0' && str[i + 1] != '\n')
		{
			++i;
			if (str[i - 1] == ' ' && str[i] == ' ') continue;
			fputc(str[i], out);
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
