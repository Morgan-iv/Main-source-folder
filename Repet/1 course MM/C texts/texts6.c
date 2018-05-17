#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE * in = fopen("input.txt", "r");
	FILE * out = fopen("output.txt", "w");
	int c, i;
	char * chr1, * chr2, * str = calloc(4008, sizeof(char));
	str[0] = ' ';
	while (fgets(&(str[1]), 4004, in))
	{
		for(i = 0; str[i] != '\0'; ++i)
			1;
		if (str[i - 1] == '\n') --i; 
		str[i + 2] = str[i + 1];
		str[i + 1] = str[i];
		str[i] = ' ';
		chr1 = strstr(str, " the ");
		chr2 = strstr(str, " a ");
		for(i = 0; str[i] != '\0'; ++i)
		{
			if (&(str[i]) == chr1)
			{
				fprintf(out, "%s", " THE");
				i += 3;
				chr1 = strstr(&(str[i]), " the ");
				continue;
			}
			if (&(str[i]) == chr2)
			{
				fprintf(out, "%s", " ANY");
				i += 1;
				chr2 = strstr(&(str[i]), " a ");
				continue;
			}
			fputc(str[i], out);
		}
		free(str);
		str = calloc(4008, sizeof(char));
		str[0] = ' ';
	}
	fclose(in);
	fclose(out);
	return 0;
}