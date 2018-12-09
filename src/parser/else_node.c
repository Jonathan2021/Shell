/**
 ** \file parser/else_node.c
 ** \brief check else grammar and create else node
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief init else node
 ** \param token linked list
 ** \return node else
 **/

struct AST *else_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

/**
 ** \brief init elif node
 ** \param token linked list
 ** \return node elif
 **/

struct AST *elif_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_if;
    return node;
}

/**
 ** \brief check grammar else_clause and create else_clause node
 ** \param token linked list
 ** \return node else_clause
 **/

struct AST *else_clause(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *elif = NULL;
    if (strcmp(tmp->name, "else") == 0)
    {
        next_token(&tmp);
        if ((elif = compound_list(&tmp)) != NULL)
        {
            *t = tmp;
            return elif;
        }
    }
    tmp = *t;
    if (strcmp(tmp->name, "elif") == 0)
    {
        next_token(&tmp);
        struct AST *condition = NULL;
        struct AST *elif_body = NULL;
        struct AST *else_body = NULL;
        if ((condition = compound_list(&tmp)) == NULL)
            return NULL;
        if (tmp == NULL)
            tmp = call_ps2(t);

        if (strcmp("then", tmp->name) != 0)
        {
            AST_destroy(condition);
            return NULL;
        }
        next_token(&tmp);

        if ((elif_body = compound_list(&tmp)) == NULL)
        {
            AST_destroy(condition);
            return NULL;
        }
        if (tmp == NULL)
            tmp = call_ps2(t);

        else_body = else_clause(&tmp);
        *t = tmp;
        struct AST *elif = elif_init(*t);
        elif->child[0] = condition;
        elif->child[1] = elif_body;
        elif->child[2] = else_body;
        return elif;
    }
    else
        return NULL;
}
