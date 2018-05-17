#include<stdio.h>
#include<stdlib.h>

double fun(double x, double y)
{
	return (x + y) * (x + y);
}

int main()
{
	int i;
	double h = 0.1;
	double f[6];
	double y[6];
	y[0] = 1;
	f[0] = fun(0, y[0]);
	for(i = 1; i < 6; ++i)
	{
		y[i] = y[i - 1] + h * f[i - 1];
		f[i] = fun(i * h, y[i]);
	}
	for(i = 0; i < 6; ++i)
	{
		printf("i=%d x=%f y=%f f(x,y)=%f\n", i, h * i, y[i], f[i]);
	}
}