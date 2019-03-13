#define USE_COMPLEX
#include "matrix.c"

int main(int argc, char const *argv[])
{
    TYPE ** arr1;
    TYPE ** arr2;
    int i, j;
    int n1, m1;
    int n2, m2;
    FILE * f;
    matrix_t mat1;
    matrix_t mat2;
    matrix_t mat3;


    //f = fopen("ind.txt", "r");
    f = stdin;


    //__asm__ __volatile__("int3");

    fscanf(f, "%d %d", &n1, &m1);
    arr1 = malloc(n1 * sizeof(TYPE *));
    for (i = 0; i < n1; ++i)
    {
        arr1[i] = malloc(m1 * sizeof(TYPE));
        
        for (j = 0; j < m1; ++j)
        {
            FSCAN(f, arr1[i][j]);
        }
    }

    //__asm__ __volatile__("int3");

    fscanf(f, "%d %d", &n2, &m2);
    arr2 = malloc(n2 * sizeof(TYPE *));
    for (i = 0; i < n2; ++i)
    {
        arr2[i] = malloc(m2 * sizeof(TYPE));
        
        for (j = 0; j < m2; ++j)
        {
            //fscanf(f, " (%lf, %lf) ", &((arr2[i][j]).re), &((arr2[i][j]).im));
            FSCAN(f, arr2[i][j]);
        }
    }

    //fclose(f);


    


    mat1 = create_matrix(n1, m1, arr1);
    mat2 = create_matrix(n2, m2, arr2);
    mat_print(mat1);
    printf("\n");
    mat_print(mat2);
    printf("\n");
    mat3 = mat_mult(mat1, mat2);
    mat_print(mat3);
    printf("\n");


    delete_matrix(&mat1);
    delete_matrix(&mat2);
    delete_matrix(&mat3);

    return 0;
}