#include <stdio.h>
#include <stdlib.h>

typedef struct complex
{
    double re, im;
} complex_t;

typedef struct matrix
{
    int size_n, size_m;
    int type; //[1]-double;[2]-complex
    void* data;
} matrix_t;

matrix_t *new_matrix_double(int type,int size_n,int size_m)
{
    matrix_t*m;
    m=(matrix_t*)malloc(sizeof(matrix_t));
    m->type=type;
    m->size_n=size_n;
    m->size_m=size_m;
    m->data=malloc(sizeof(double)*m->size_m*m->size_n);
    return m;
}
void creat_matrix_double(matrix_t*m,int size_m, int size_n)
{
    for (int i=0;i<m->size_n;i++)
    {
        for (int j=0;j<m->size_m;j++)
        {
            printf("Enter %d%d componets of matrix:",i,j);
            scanf("%lf",(double*)m->data+(i*m->size_m)+j);
        }
    }
}
void print_matrix_double(matrix_t*m)
{
     for(int i=0;i<m->size_n;i++)
     {
        for(int j =0;j<m->size_m;j++)
        {
            printf("%10.5lf",*((double*)m->data+(i*m->size_m)+j));
        }
        printf("\n");
      }
}
matrix_t *sum_matrix_double(matrix_t*a,matrix_t*b)
{
    int size_n=a->size_n;
    int size_m=a->size_m;
    matrix_t*res=new_matrix_double(a->type,size_n,size_m);
    for (int i=0;i<size_n;i++)
    {
        for (int j=0;j<size_m;j++)
        {
            *((double*)res->data+(i*size_m)+j)=*((double*)a->data+(i*size_m)+j)+*((double*)b->data+(i*size_m)+j);
        }
    }
    return res;
}

matrix_t *multi_matrix_double(matrix_t*a,matrix_t*b)
{
    int size_k=a->size_m;
    int size_n=a->size_n;
    int size_m=b->size_m;
    matrix_t*res=new_matrix_double(a->type,size_n,size_m);
    for (int i=0;i<size_n;i++)
    {
        for (int j=0;j<size_m;j++)
        {
            for (int r=0;r<size_k;r++)
            {
                *((double*)res->data+(i*size_m)+j)+=(*((double*)a->data+(i*size_k)+r))*(*(double*)b->data+(r*size_m)+j);
           }

        }

    }
 return res;
}

int main (int argc, char const *argv[])
{
    int size_n,size_m,size_k;
    int type, menu;
    matrix_t*m1;
    matrix_t*m2;

    printf("Choice the item\n");
    printf("\n");
    printf("1.Sum matrix\n");
    printf("2.Multiply matrix\n");
    printf("3.Transposition matrix\n");
    printf("\n");
    printf("Enter item:");
    scanf("%d",&menu);
    switch(menu)
    {
    case 1:
        {
            printf("Enter the size of matrix\n");
            printf("Enter the colum of matrix:\n");
            scanf("%d",&size_n);
            printf("Enter the line of matrix:\n");
            scanf("%d",&size_m);
            if (size_m <=0 || size_n <=0)
            {
                printf("ERROR: The size of the matrix cannot be <=0");
                return 0;
            }
            printf("For work with double numbers choose 1, for work with complex - 2, for testing - 3  -> ");
            scanf("%d", &type);
            if (type !=1 && type !=2)
            {
                printf("ERROR: The type was entered incorrectly");
                return 0;
            }
                switch (type)
                {
                    case 1:
                    {
                        m1=new_matrix_double(type,size_n,size_m);
                        creat_matrix_double(m1,size_m,size_n);
                        print_matrix_double(m1);
                        m2=new_matrix_double(type,size_n,size_m);
                        creat_matrix_double(m2,size_m,size_n);
                        print_matrix_double(m2);

                        printf("Result:");
                        printf("\n");
                        matrix_t*m3=sum_matrix_double(m1,m2);
                        print_matrix_double(m3);
                        return 0;
                    }

                    case 2:
                    {
                    }
                }
        }
    case 2:
        {
            printf("Enter the size of matrix\n");
            printf("Enter the colum of matrix1:\n");
            scanf("%d",&size_n);
            printf("Enter the line of matrix1:\n");
            scanf("%d",&size_m);
            printf("M1=N1");
            printf("\n");
            printf("Enter the line of matrix2:\n");
            scanf("%d",&size_k);

            if (size_m <=0 || size_n <=0 || size_k <=0)
            {
                printf("ERROR: The size of the matrix cannot be <=0");
                return 0;
            }

            printf("For work with double numbers choose 1, for work with complex - 2, for testing - 3  -> ");
            scanf("%d", &type);

            if (type !=1 && type !=2)
            {
                printf("ERROR: The type was entered incorrectly");
                return 0;
            }

            switch (type)
            {
                case 1:
                {
                    m1=new_matrix_double(type,size_n,size_k);
                    creat_matrix_double(m1,size_k,size_n);
                    print_matrix_double(m1);
                    m2=new_matrix_double(type,size_k,size_m);
                    creat_matrix_double(m2,size_m,size_k);
                    print_matrix_double(m2);

                    printf("Result:");
                    printf("\n");
                    matrix_t*m3=multi_matrix_double(m1,m2);
                    print_matrix_double(m3);
                    return 0;
                }
                case 2:
                    {

                    }
            }
            }}}






