/**
 ** \file parser/my_tree.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "../include/shell.h"

/**
 ** \brief checks if a string is a quote
 ** \param  str string to check
 ** \return  returns 1 if it is a quote, 0 if not
 **/

int is_quoted(char *str)
{
    if(!str)
        return 0;
    size_t len = strlen(str);
    if(len <= 1)
        return 0;
    char f = str[0];
    return ((f == 39 || f == '"') && f== str[len - 1]);
}

/**
 ** \brief checks if a string is a quote
 ** \param  str string to check
 ** \return  returns 1 if it is a quote, 0 if not
 **/
void my_itoa(char str[], int nb, int size)
{
    str[size--] = 0;
    for (; size >= 0; --size)
    {
        str[size] = nb % 10 + '0';
        nb = nb / 10;
    }
}

/**
 ** \brief counts the number of digits in a number
 ** \param  nb number to count
 ** \return  returns number of digits in nb
 **/
int size_int(int nb)
{
    int i = 1;
    for (int j = 10; nb / j ; ++i, j *= 10);
    return i;
}

/**
 ** \brief checks if the string is one of the delimiters
 ** \param  str string to check
 ** \return  returns 1 if it is one of the delimiters, 0 if not
 **/
int is_delim(char *str)
{
    return (!strcmp(str, ";") || !strcmp(str, "\n") || !strcmp(str, "&"));
}

/**
 ** \brief sets ? environment variable to the desired value
 ** \param  int res value to be set
 **/
void set_status(int res)
{
    int size = size_int(res);
    char str[size + 1];
    my_itoa(str, res, size);
    setvalue("?", str);
}

/**
 ** \brief evaluates node->res
 ** \param  node node which res will be evaluated
 ** \return  returns 1 if node->res == 0, 0 if not
 **/

int eval_node(struct AST *node)
{
    return !node->res;
}

/**
 ** \brief create node ast with number of child
 ** \param  child number of child
 ** \return node ast
 **/

struct AST *AST_init(int nb_child)
{
    struct AST *new = malloc(sizeof(struct AST));
    if (new == NULL)
        return NULL;
    new->child = malloc(sizeof(struct AST *) * nb_child);
    new->nb_child = nb_child;
    for (int i = 0; i < nb_child; i++)
    {
        new->child[i] = NULL;
    }
    new->res = 0;
    new->foo = NULL;
    return new;
}

/**
 ** \brief free ast
 ** \param  s ast tree
 **/

void AST_destroy(struct AST *s)
{
    if (s == NULL)
        return;
    if (!strcmp(s->self->type, "ASSIGMENT_WORD"))
    {
        free(s->self->name);
        free(s->self);
    }
    else if (strcmp(s->self->name, "list") == 0
             || strcmp(s->self->name, "simple command") == 0
             || strcmp(s->self->name, "command") == 0
             || strcmp(s->self->name, "compound") == 0
             || strcmp(s->self->name, "SHELL COMMAND") == 0)
        free(s->self);
    for (int i = 0; i < s->nb_child; i++)
    {
        AST_destroy(s->child[i]);
    }

    free(s->child);
    free(s);
}

/**
 ** \brief add a child on l node
 ** \param  l node ast
 ** \param  a node ast
 **/

void add_list(struct AST *l, struct AST *a)
{
    l->nb_child++;
    l->child = realloc(l->child, l->nb_child * sizeof(struct AST *));
    l->child[l->nb_child - 1] = a;
}

/**
 ** \brief free child and name
 ** \param ast tree
 **/

void free_l(struct AST *ast)
{
    free(ast->self);
    free(ast->child);
    free(ast);
}
