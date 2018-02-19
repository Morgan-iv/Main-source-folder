#include <stdlib.h>
#include <stdio.h>

typedef struct str_stack
{
	int * data;
	int top;
	int capacity;
} stack;

stack * get_stack(stack * st)
{
	if (st == NULL)
	{
		st = (stack *)malloc(sizeof(stack));
		st->capacity = 10;
		st->top = 0;
		st->data = (int *)malloc(sizeof(int) * st->capacity);
	}
	else if (st->top == st->capacity)
		st->data = (int *)realloc(st->data, (st->capacity *= 2) * sizeof(int));
	return st;
}

int push(stack * st, int elem)
{
	if (st == NULL) return 1;
	if (st->top == st->capacity)
		get_stack(st);
	st->data[st->top++] = elem;
	return 0;
}

int pop(stack * st)
{
	if (st == NULL) return 1;
	if (st->top == 0) return 0;
	return st->data[--st->top];
}

int peek(stack * st)
{
	if (st == NULL) return 1;
	if (st->top == 0) return 0;
	return st->data[st->top - 1];
}

int destruct(stack * st)
{
	if (st == NULL) return 1;
	free(st->data);
	free(st);
	return 0;
}
