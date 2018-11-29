/**
 ** \file parser/element.c
 ** \brief check element grammar and create element node
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief init element node
 ** \param token linked list
 ** \return node element
 **/

struct AST *element_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

/**
 ** \brief check grammar element and create element node
 ** \param token linked list
 ** \return node element
 **/

struct AST *element(struct Token **t)
{
    struct AST *redir;
    if (strcmp(t[0]->type, "WORD") == 0)
    {
        struct AST *word = element_init(*t);
        *t = t[0]->next;
        return word;
    }
    else if ((redir = redirection(t)) != NULL)
    {
        return redir;
    }
    else
        return NULL;
}
