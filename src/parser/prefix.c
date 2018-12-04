/**
 ** \file parser/prefix.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

void foo_assigment(struct AST *node, __attribute__((unused))struct fds fd)
{
    if(!node)
        return;
    int i = 0;
    for(; node->self->name[i] != '='; ++i);
    node->self->name[i] = 0;
    setvalue(node->self->name, getvalue(node->self->name + i + 1));
}

/**
 ** \brief init assigment node
 ** \param token linked list
 ** \return node assigment
 **/

struct AST *assigment_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if(!node)
        return NULL;
    token->type = "ASSIGMENT_WORD";
    node->self = token;
    node->foo = foo_assigment;
    return node;
}
/**
 ** \brief check grammar prefix and create prefix node
 ** \param token linked list
 ** \return node prefix
 **/

int is_assigment(char *str)
{
    if (!str || str[0] == '=' || (str[0] > '0' && str[0] < '9'))
        return 0;
    int i = 0;
    for (; str[i] != '=' && str[i] != 0; ++i);
    return (str[i] == '=');
}

struct AST *prefix(struct Token **t)
{
    struct AST *redir;
    if (is_assigment(t[0]->name))
    {
        struct AST *assigment_word = assigment_init(*t);
        *t = t[0]->next;
        return assigment_word;
    }
    else if ((redir = redirection(t)) != NULL)
    {
        return redir;
    }
    else
        return NULL;
}
