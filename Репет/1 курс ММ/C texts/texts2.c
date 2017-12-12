#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i = 1;
	char str[4005];
	while (fgets(str, 4004, in))
	{
		fprintf(out, "%d %s", i, str);
		++i;
	}
	fclose(in);
	fclose(out);
	return 0;
}