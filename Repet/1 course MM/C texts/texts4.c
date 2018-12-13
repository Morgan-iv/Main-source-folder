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
		c = 0;
		for(i = 0; str[i] != '\0'; ++i)
		{
			if (c == ' ' && (str[i] != ' ' && str[i] != '\n'))
				fputc('[', out);
			if (c != ' ' && (str[i] == ' ' || str[i] == '\n'))
				fputc(']', out);
			fputc(str[i], out);
			c = str[i];
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
