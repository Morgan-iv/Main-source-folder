#include"client.h"
#include<stdio.h>
#include<unistd.h>

int main()
{
	int a, i;
	char * p;
	IPCinit();
	a = 2;
	p = GetStr(&a);
	if (p)
	{
		p[0] = 'b';
		p[1] = '\0';
	}
	for (i = 0; i < 10; ++i)
	{
		putc('0' + i, stdout);
		sleep(1);
		fflush(stdout);
	}
	a = 1;
	p = NULL
	while (!p)
	{
		p = GetStr(&a);
		sleep(1);
	}
	putc(p[0], stdout);
	FreeStr(&a);
	a = 2;
	FreeStr(&a);
	IPCexit();
	putc('\n', stdout);
	return 0;
}