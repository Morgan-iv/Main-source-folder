#include<cstdio>

using namespace std;

int main()
{
    FILE * rc = fopen("coordtable.dat", "wb");
    FILE * pt = fopen("shifts.d", "r");
    
    double s;
    int h, ll;
    int c = 0;
	short l;
	long long ts;
    
    while (fscanf(pt, "%lf %d %d", &s, &h, &ll) == 3)
    {
        ts = 1 + (long long)s;
		l = ((short)ll) * 2;
		h *= 2;
        ++c;
        fwrite(&ts, sizeof(long long), 1, rc);
		fwrite(&h, sizeof(int), 1, rc);
		fwrite(&l, sizeof(short), 1, rc);
    }
    printf("%d\n", c);
    return 0;
}
