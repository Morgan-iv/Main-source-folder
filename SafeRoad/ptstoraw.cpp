#include<cstdio>

using namespace std;

int main()
{
    FILE * rc = fopen("rawcoords.dat", "wb");
    FILE * pt = fopen("points.d", "r");
    
    double s;
    float p;
    int c = 0;
    
    while (fscanf(pt, "%lf", &s) == 1)
    {
        p = (float)s;
        ++c;
        fwrite(&p, sizeof(float), 1, rc);
    }
    printf("%d", c);
    return 0;
}

//185736
