# 1 "matrix.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "matrix.c"



typedef struct complex
{
    double re, im;
} complex_t;
# 33 "matrix.c"
typedef struct matrix
{
    int n, m;
    double ** data;
} matrix_t;

matrix_t create_matrix(int n, int m, double ** data)
{
    matrix_t res;
    int i, j;

    res.n = n;
    res.m = m;
    res.data = NULL;

    if (n <= 0 || m <= 0)
        return res;

    res.data = malloc(n * sizeof(double *));
    for (i = 0; i < n; ++i)
    {
        res.data[i] = malloc(m * sizeof(double));
    }

    if (data == NULL)
        return res;

    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < m; ++j)
        {
            res.data[i][j] = data[i][j];
        }
    }
    return res;
}

void delete_matrix(matrix_t * mat)
{
    int i;
    for (i = 0; i < mat->n; ++i)
    {
        free(mat->data[i]);
        mat->data[i] = NULL;
    }
    free(mat->data);
    mat->data = NULL;
    mat->n = 0;
    mat->m = 0;
}

matrix_t mat_plus(matrix_t a, matrix_t b)
{
    matrix_t res;
    int i, j;

    if (a.n <= 0 || a.m <= 0 || b.n <= 0 || a.m <= 0)
        return create_matrix(0, 0, NULL);

    if (a.n != b.n || a.m != b.m)
        return create_matrix(0, 0, NULL);

    if (a.data == NULL || b.data == NULL)
        return create_matrix(-1, -1, NULL);

    res = create_matrix(a.n, a.m, NULL);

    for (i = 0; i < res.n; ++i)
    {
        for (j = 0; j < res.m; ++j)
        {
            res.data[i][j] = ((a.data[i][j]) + (b.data[i][j]));
        }
    }

    return res;
}

matrix_t mat_mult(matrix_t a, matrix_t b)
{
    matrix_t res;
    int i, j, k;


    if (a.n <= 0 || a.m <= 0 || b.n <= 0 || a.m <= 0)
        return create_matrix(0, 0, NULL);

    if (a.m != b.n)
        return create_matrix(0, 0, NULL);

    if (a.data == NULL || b.data == NULL)
        return create_matrix(-1, -1, NULL);

    res = create_matrix(a.n, b.m, NULL);

    for (i = 0; i < res.n; ++i)
    {
        for (j = 0; j < res.m; ++j)
        {
            res.data[i][j] = 0;
            for (k = 0; k < a.m; ++k)
            {
                res.data[i][j] = ((res.data[i][j]) + (((a.data[i][k]) * (b.data[k][j]))));
            }
        }
    }

    return res;
}

matrix_t mat_tran(matrix_t mat)
{
    matrix_t res;
    int i, j;

    if (mat.n <= 0 || mat.m <= 0)
        return create_matrix(0, 0, NULL);

    if (mat.data == NULL)
        return create_matrix(-1, -1, NULL);

    res = create_matrix(mat.m, mat.n, NULL);

    for (i = 0; i < res.n; ++i)
    {
        for (j = 0; j < res.m; ++j)
        {
            res.data[i][j] = mat.data[j][i];
        }
    }

    return res;
}

void mat_print(matrix_t mat)
{
    int i, j;

    if (mat.n <= 0 || mat.m <= 0)
    {
        printf("Empty matrix\n");
        return;
    }

    if (mat.data == NULL)
    {
        printf("Bad pointer\n");
        return;
    }

    printf("%d %d\n", mat.n, mat.m);
    for (i = 0; i < mat.n; ++i)
    {
        for (j = 0; j < mat.m; ++j)
        {
            printf("%9.3f ", (mat.data[i][j]));
        }
        printf("\n");
    }
}
