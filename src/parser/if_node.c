/**
 ** \file parser/if_node.c
 ** \brief check if grammar and create if node
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"

/**
 ** \brief execute if function
 ** \param node ast tree for execution
 ** \param file descriptor
 **/

void foo_if(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0])
        return;
    node->child[0]->foo(node->child[0], fd);
    node->res = node->child[0]->res;
    if(node->res && node->nb_child > 1 && node->child[1])
        node->child[1]->foo(node->child[1], fd);
    else if(!node->res && node->nb_child > 2 && node->child[2])
        node->child[2]->foo(node->child[2], fd);
}

/**
 ** \brief init if node
 ** \param token linked list
 ** \return node if
 **/

struct AST *if_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if(!node)
        return NULL;
    node->self = token;
    token->type = "IF";
    node->foo = foo_if;
    return node;
}

/**
 ** \brief check grammar if and create if node
 ** \param token linked list
 ** \return node if
 **/

struct AST *rule_if(struct Token **t)
{
    struct AST *condition;
    struct AST *if_body;
    struct AST *else_body;
    struct Token *name;

    struct Token *tmp = *t;
    if (strcmp("if", tmp->name) != 0)
        return NULL;
    name = tmp;
    next_token(&tmp);

    if (tmp == NULL || (condition = compound_list(&tmp)) == NULL)
        return NULL;

    if (tmp == NULL)
        tmp = call_ps2(t);

    if (strcmp("then", tmp->name) != 0)
    {
        AST_destroy(condition);
        return NULL;
    }
    next_token(&tmp);

    if ((if_body = compound_list(&tmp)) == NULL)
    {
        AST_destroy(condition);
        return NULL;
    }
    if (tmp == NULL)
        tmp = call_ps2(t);

    else_body = else_clause(&tmp);
    if (tmp == NULL)
        tmp = call_ps2(t);

    if (strcmp("fi", tmp->name) != 0)
    {
        AST_destroy(condition);
        AST_destroy(if_body);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = if_init(name);
    node->child[0] = condition;
    node->child[1] = if_body;
    node->child[2] = else_body;
    return node;
}
