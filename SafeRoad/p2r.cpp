#include<cstdio>

using namespace std;

int main()
{
    FILE * rc = fopen("rawcoords.dat", "wb");
    FILE * pt = fopen("points.d", "r");
    
    double s;
    float p;
    int c = 0;
    
	printf("%d\n", sizeof(float));
	
    while (fscanf(pt, "%lf", &s) == 1)
    {
        p = (float)s;
		//printf ("%f %f\n", p, s);
        ++c;
		if (c == 11) scanf("");
        fwrite(&p, sizeof(float), 1, rc);
    }
    printf("%d\n", c);
    return 0;
}

//185736
