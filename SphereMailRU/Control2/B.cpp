#include<set>
#include<cstdio>

using namespace std;

typedef unsigned int uint;
typedef long long int64;

int main()
{
	int64 a, n, b, s2 = 0;
	//double s = 0;
 	scanf("%lld", &n);
	multiset<int64> arr;
 	for (int i = 0; i < n; ++i)
	{
		scanf("%lld", &a);
		arr.insert(a);
	}
	for (int i = 1; i < n; ++i)
	{
		a = *arr.begin();
		arr.erase(arr.begin());
		b = *arr.begin();
		arr.erase(arr.begin());
		s2 += (a + b);
		arr.insert(arr.begin(), a + b);
	}
	printf("%.2f", (s2) * 0.01);
	return 0;
}