#include<stdio.h>
#include<math.h>

#define EPS 0.00001

double abs(double x)
{
	return (x < 0) ? -x : x;
}

double findroot(double a, double b, double (*fun)(double))
{
	double m
	while(b - a > EPS)
	{
		m = (a + b) / 2;
		if ((*fun)(m) > 0) b = m;
		else a = m;
	}
}

double integrate(double a, double b, double (*fun)(double))
{
	double f, x, h, s, res;
	int i, n;
	n = 5;
	h = (b - a) / n; 
	res = 0;
	while(abs(res - s) > EPS)
	{
		n *= 2;
		h *= 0.5;
		x = a + h;
		s = 0;
		for(i = 1; i < n; ++i)
		{
			s += (*fun)(x);
			x += h;
		}
		s += 0.5 * ((*fun)(a) + (*fun)(b));
		s *= h;
	}
}

double f1(double x)
{
	return exp(-x * x) - x;
}

double f2(double x)
{
	return exp(-x * x) - x * x;
}

double f3(double x)
{
	return exp(-x * x) - x - x * x;
}

double f4(double x)
{
	return exp(-(x - 0.5) * (x - 0.5)) - x * x;
}

double f5(double x)
{
	return x - 0.5 * sin(x) - 1;
}

double f6(double x)
{
	return 2 * x * x * x + 4 * x - 1;
}

double f1(double x)
{
	return exp(-x) - 0.5 - sqrt(x);
}

double i1(double x)
{
	return pow(x + 1, 1.0/3);
}

int main()
{
	printf("%f", integrate(0, 1.2, i1));
	return 0;
}