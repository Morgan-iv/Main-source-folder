#include<stdio.h>
#include<stdlib.h>
//#include"strhead.h"
#include<time.h>

struct str_ 
{ 
	int i;
	double d; 
};

int f1(struct str_ * s)
{
	printf("i= %d d= %f\n", s->i, s->d);
	return 0;
}

int f2(struct str_ * a, int n)
{
	int i;
	for(i = 0; i < n; ++i)
	{
		printf("i[%d]= %d d[%d]= %f\n", i, a[i].i, i, a[i].d);
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	struct str_ s;
	struct str_ a[10];
	int i;
	s.i = 2;
	s.d = 3.14;
	for(i = 0; i < 10; ++i)
	{
		a[i].i = i;
		a[i].d = ((double)rand())/RAND_MAX*3.0 - 1.0;
	}
	f1(&s);
	f2(a, 10);
	return 0;
}