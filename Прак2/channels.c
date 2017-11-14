#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char * argv[])
{
	int strch[2];
	int flags[2];
	pid_t child;
	char str[21];
	int flag;
	FILE * out = fopen(argv[1], "w");
	
	pipe(strch);
	pipe(flags);
	if ((child = fork()) == -1)
	{
		perror("fork");
		return 1;
	}
	if (child == 0)
	{
		close(strch[1]);
		close(flags[0]);
		while(1)
		{
			read(strch[0], str, 21);
			if (flag = !!strcmp(str, "exit\n"))
				fputs(str, out), fflush(out);
			write(flags[1], &flag, 1);
			if (!flag) return 0;
		}
	}
	else
	{
		close(strch[0]);
		close(flags[1]);
		flag = 1;
		while(flag)
		{
			fgets(str, 21, stdin);
			write(strch[1], str, 21);
			read(flags[0], &flag, 1);
			if (flag)
				fputs("==========\n", out), fflush(out);
		}
		fclose(out);
		return 0;
	}
}