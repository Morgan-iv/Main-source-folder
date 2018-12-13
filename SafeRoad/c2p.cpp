#include<cstdio>

using namespace std;

int main()
{
    FILE * rc = fopen("probatable.dat", "wb");
    FILE * pt = fopen("columns.d", "r");
    
    double s;
    int c = 0;
	float p;
	long long ts;
    
    while (fscanf(pt, "%lf", &s) == 1)
    {
        ts = 1 + (long long)s;
		fwrite(&ts, sizeof(long long), 1, rc);
		++c;
		for (int i = 0; i < 20; ++i)
		{
			fscanf(pt, "%lf", &s);
			p = float(s);
			fwrite(&p, sizeof(float), 1, rc);
		}
    }
    printf("%d\n", c);
    return 0;
}
