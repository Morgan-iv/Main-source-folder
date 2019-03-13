#include <stdlib.h>
#include <stdio.h>

typedef struct complex
{
    double re, im;
} complex_t;

#if !defined(USE_DOUBLE) && !defined(USE_COMPLEX)
#define USE_DOUBLE
#endif

#ifdef USE_DOUBLE
#undef USE_COMPLEX
#define TYPE double
#define PLUS(a, b) ((a) + (b)) 
#define MULT(a, b) ((a) * (b))
#define PRINT(a) printf("%9.3f ", (a))
#define FSCAN(f, a) fscanf(f, "%lf", &(a))
#define ZERO 0
#endif

#ifdef USE_COMPLEX
#undef USE_DOUBLE
#define TYPE complex_t
#define PLUS(a, b) ((complex_t) {(a).re + (b).re, (a).im + (b).im})
#define MULT(a, b) ((complex_t) {(a).re * (b).re - (a).im * (b).im, (a).re * (b).im + (a).im * (b).re})
#define PRINT(a) printf("(%9.3f, %9.3f) ", (a).re, (a).im)
#define FSCAN(f, a) fscanf(f, " (%lf, %lf) ", &((a).re), &((a).im))
#define ZERO ((complex_t) {0, 0})
#endif

typedef struct matrix
{
    int n, m;
    TYPE ** data;
} matrix_t;

matrix_t create_matrix(int n, int m, TYPE ** data)
{
    matrix_t res;
    int i, j;

    res.n = n;
    res.m = m;
    res.data = NULL;

    if (n <= 0 || m <= 0)
        return res;

    res.data = malloc(n * sizeof(TYPE *));
    for (i = 0; i < n; ++i)
    {
        res.data[i] = malloc(m * sizeof(TYPE));
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
            res.data[i][j] = PLUS(a.data[i][j], b.data[i][j]);
        }
    }

    return res;
}

matrix_t mat_mult(matrix_t a, matrix_t b)
{
    matrix_t res;
    int i, j, k;
    /*TYPE tmp;*/

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
            res.data[i][j] = ZERO;
            for (k = 0; k < a.m; ++k)
            {
                res.data[i][j] = PLUS(res.data[i][j], MULT(a.data[i][k], b.data[k][j]));
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
            PRINT(mat.data[i][j]);
        }
        printf("\n");
    }
}



