#include<cstdio>
#include<set>
#include<vector>
#include<algorithm>
 
using namespace std;
 
int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    int num;
    set<vector<int>>cnt;
    vector<int>temp(m);
    for (int i = 0; i < n; i++) {
        temp.clear();
        for (int j = 0; j < m; j++) {
            scanf("%d", &num);
            temp.push_back(num);
        }
        sort(begin(temp), end(temp));
        cnt.insert(temp);
    }
    for (int i = 0; i < k; i++) {
        temp.clear();
        for (int j = 0; j < m; j++) {
            scanf("%d", &num);
            temp.push_back(num);
        }
        sort(begin(temp), end(temp));
        if (cnt.find(temp) != cnt.end()) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
    return 0;
}
