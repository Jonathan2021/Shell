/**
 ** \file parser/do_group.c
 ** \brief do grammar and create node "do"
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief init do node
 ** \param token linked list
 ** \return node do
 **/

struct AST *do_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

/**
 ** \brief check grammar do_group and create do node
 ** \param token linked list
 ** \return node do
 **/

struct AST *do_group(struct Token **t)
{
    struct AST *do_grp;
    struct Token *tmp = *t;
    if (strcmp(tmp->name, "do"))
        return NULL;
    next_token(&tmp);
    if ((do_grp = compound_list(&tmp)) == NULL)
        return NULL;
    if (tmp == NULL)
        tmp = call_ps2(t);
    if (strcmp(tmp->name, "done"))
    {
        AST_destroy(do_grp);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    return do_grp;
}
