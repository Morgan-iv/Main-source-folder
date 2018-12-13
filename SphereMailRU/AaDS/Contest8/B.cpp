#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

enum { MAX_LEN = 10 };

int main(void)
{
    int n, k;
    cin >> n >> k;
    char tmp[MAX_LEN];
    sprintf(tmp, "%d", k);
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        char buf[MAX_LEN];
        sprintf(buf, "%d", i);
        ans += (strcmp(buf, tmp) <= 0);
    }
    cout << ans << endl;
    return 0;
}
