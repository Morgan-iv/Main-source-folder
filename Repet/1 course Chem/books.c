#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int n, l, i, j, k;
    int * arr;
    FILE * f;
    f = fopen("input.txt", "r");
    fscanf(f, "%d %d", &n, &l);
    arr = malloc(n * sizeof(int));
    for (i = 0; i < n; ++i)
    {
        fscanf(f, "%d", &(arr[i]));
    }
    fclose(f);
    for (i = 0; i < n; ++i)
    {
        k = -1;
        for (j = 0; j < n; ++j)
        {
            if (arr[j] <= l && arr[j] > 0 && (k == -1 || (k >= 0 && arr[k] > arr[j])))
            {
                k = j;
            }
        }
        if (k == -1)
            break;
        l -= arr[k];
        arr[k] = -1;
        printf("%d ", k);
    }
    free(arr);
    printf("\n");
    return 0;
}