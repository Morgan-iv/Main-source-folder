#include"client.h"

int main()
{
	int a;
	char * p;
	IPCinit();
	a = 1;
	p = GetStr(&a);
	if (p)
	{
		p[0] = 'a';
		p[1] = '\0';
		putc('k', stdout);
	}
	else
		putc('z', stdout);
	FreeStr(&a);
	IPCexit();
	return 0;
}
