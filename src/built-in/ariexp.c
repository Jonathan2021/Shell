#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

static char **split(char *str, char delim)
{
    char **rslt = malloc(sizeof(char *) * strlen(str));
    int i = 0;
    int gi = 0;
    for (; str[i] != '\0'; i++)
    {
        char *tmp = malloc(strlen(str));
        int j = 0;
        for (; str[i] != delim && str[i] != '\0'; j++, i++)
        {
            if (str[i] != ' ')
                tmp[j] = str[i];
            else
                j--;
        }
        tmp[j] = '\0';
        rslt[gi++] = tmp;
        if (str[i] == '\0')
            break;
    }
    rslt[gi] = '\0';
    return rslt;
}

/*
   static char **splitting(char *str, char *delim)
   {
   char **rslt = malloc(sizeof(char *) * strlen(str));
   int i = 0;
   int g_i = 0;
   for (; str[i] != '\0'; i++)
   {
   char *tmp = malloc (strlen(str));
   int j = 0;
   for (int k = 0; delim[k] != '\0' && delim[k] ','; k++)
   {
   for (; str[i] != '\0' && str[i] != delim[k]; j++)
   {
   if (str[i] != ' ')
   tmp[] = str[i];
   }
   }
   }
   }*/




int is_arithmetic(char *str)
{
    if (str[0] != '\0' && str[0] == '$')
    {

        if (str[1] != '\0' && str[1] == '(')
        {
            if (str[2] != '\0' && str[2] == '(')
            {
                return 1;
            }
        }
    }
    return 0;
}
static char *getvalue(char *name)
{
    if (name[0] == '$')
        name = getenv(name + 1);
    else
    {
        if (name[0] < '0' && name[0] > '9')
        {
            char *tmp = getenv(name);
            if (tmp != NULL)
                name = tmp;
        }
    }
    return name;
}

static void del_split(char **cut)
{
    if (cut != NULL)
    {
        for (int i = 0; cut[i] != NULL ; i++)
            if (cut[i] != NULL)
                free(cut[i]);
        free(cut);
    }
}

int expen(char *str)
{
    char **cut = split(str, '+');
    int rslt = 0;
    if (cut[1] == NULL)
        printf("NOT GOOD\n");
    else
    {
        if (cut[0][0] == '\0')
            cut[0] = calloc(1, 4096);
        for (int j = 0; cut[j] != NULL; j++)
        {
            char *str = getvalue(cut[j]);
            if (str == NULL)
                str = "0";
            rslt += atoi(str);
        }

    }
    del_split(cut);
    return rslt;
}


static struct stack *st_op(struct stack *op, char ch)
{

    char *ope = malloc(3);
    ope[0] = ch;
    struct full_data p ;
    p.type = TYPE_STR;
    p.data.str = ope;
    op = push(op, p);
    return op;
}

static struct stack *st_nb(struct stack *st, char *tmp)
{
    tmp = getvalue(tmp);
    int tt = atoi(tmp);
    struct full_data nb;
    nb.type = TYPE_INT;
    nb.data.res = tt;
    st = push(st, nb);
    return st;
}

static struct tree *rec_build(struct stack *st, struct tree *t)
{
    if (st == NULL)
        return t;
    struct tree *node = malloc(sizeof(struct tree));
    node->left = NULL;
    node->right = NULL;
    node->fd = st->fd;
    if (st->fd.type == TYPE_STR)
    {
        root->fd = st->fd;
        t->left = 
        st = pop(st);
        rec_build(st, 
    }

}
static struct tree *build(struct stack *st)
{
    return rec_build(st, NULL);
}

int arith_expand(char *str)
{
    struct stack *st = stack_init();
    struct stack *op = stack_init();
    for (int i = 0; str[i] != '\0'; i++)
    {
        char *tmp = malloc(strlen(str));
        int j = 0;
        for ( ;str[i] != '\0'; i++, j++)
        {
            if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
            {
                op = st_op(op, str[i]);
                break;
            }
            if (str[i] != ' ')
                tmp[j] = str[i];
            else
                j--;
        }
        tmp[j] = '\0';
        st = st_nb(st, tmp);
        if (str[i] == '\0')
            break;
    }
    print(st);
    print(op);
    st = stack_cat(st, op);
    print(st);
    stack_destroy(st);
    stack_destroy(op);
    return 0;
}




int main(int argc, char *argv[])
{
    printf("RSLT = %d\n",arith_expand("234   * 1 - w + 3"));
    return 0;
}
