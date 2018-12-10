#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"



struct stack *stack_cat(struct stack *st1, struct stack *st2)
{
    while (st2 != NULL)
    {
        st1 = push(st1, top(st2));
        st2 = pop(st2);
    }
    stack_destroy(st2);
    return st1;
}


struct stack *stack_init(void)
{
    struct stack *st = malloc(sizeof(struct stack));
    st->next = NULL;
    struct full_data fd;
    fd.type = TYPE_STR;
    fd.data.str = NULL;
    st->fd = fd;
    return st;
}

struct stack *push(struct stack *st, struct full_data enter)
{
    if (st == NULL)
        return NULL;
    if (st->fd.type == TYPE_STR && st->fd.data.str == NULL)
        st->fd = enter;
    else
    {
        struct stack *new = malloc(sizeof(struct stack));
        new->fd = enter;
        new->next = st;
        st = new;
    }
    return st;
}
struct full_data top(struct stack *st)
{
    return st->fd;
}
void stack_destroy(struct stack *st)
{
    if (st == NULL)
        return;
    stack_destroy(st->next);
    if (st->fd.type == TYPE_STR && st->fd.data.str != NULL)
        free(st->fd.data.str);
    free(st);
}


struct stack *pop(struct stack *st)
{
    if (st == NULL)
        return NULL;
    struct stack *tmp = st;
    st = st->next;
    free(tmp);
    return st;
}
void print(struct stack *st)
{
    struct stack *tmp = st;
    for (; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->fd.type == TYPE_STR)
            printf("%s ", tmp->fd.data.str);
        else
            printf("%d ", tmp->fd.data.res);
    }
    printf("\n");
}

/*
int main()
{
    char *str = malloc(50);
    str[0] = 'k';
    str[1] = 'k';
    str[2] = 'o';
    str[3] = '\0';
    char *str2 = malloc(50);
    str2[0] = 'u';
    str2[1] = 'f';
    str2[2] = 'c';
    str2[3] = '\0';

    struct stack *st = stack_init();
    st = push(st, str);
    st = push(st, str2);
    print(st);
    printf("top = %s\n", top(st));
    st = pop(st);
    printf("top = %s\n", top(st));
    stack_destroy(st);
    free(str);
    free(str2);
}*/
