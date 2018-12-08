/**
 ** \file parser/my_tree.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "../include/shell.h"

void set_status(int res)
{
    if (res)
        setvalue("?" , "0");
    else
        setvalue("?" , "1");
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
