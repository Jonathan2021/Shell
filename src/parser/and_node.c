/**
 ** \file parser/and_node.c
 ** \brief and_node grammar and create node "and"
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief execution of node and
 ** \param node ast tree for execution
 ** \param fd  file descriptors for execution
 **/
void foo_and(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0] || !node->child[1])
        return;
    node->child[0]->foo(node->child[0], fd);
    node->res = node->child[0]->res;
    if (!eval_node(node))
        return;
    node->child[1]->foo(node->child[1], fd);
    node->res = node->child[1]->res;
}

/**
 ** \brief execution of node or
 ** \param node ast tree for execution
 ** \param fd  file descriptor for execution
 **/
void foo_or(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0] || !node->child[1])
        return;
    node->child[0]->foo(node->child[0], fd);
    node->res = node->child[0]->res;
    if (eval_node(node))
        return;
    node->child[1]->foo(node->child[1], fd);
    node->res = node->child[1]->res;
}

/**
 ** \brief init operator for and_or function and check binary operator
 ** \param node ast tree for execution
 ** \return node init of operator
 **/

struct AST *operator_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    if (!strcmp(token->name, "&&"))
        node->foo = foo_and;
    else
        node->foo = foo_or;
    return node;
}

/**
 ** \brief check and_or grammar and create node with token given in parameter
 ** \param linked list of token
 ** \return node and_or
 **/

struct AST *and_or(struct Token **t)
{
    struct AST *left_body;
    struct AST *right_body;
    struct Token *name;

    struct Token *cpy = *t;
    // int check = 2;
    if ((left_body = pipeline(&cpy)) == NULL)
        return NULL;
    *t = cpy;
    if (cpy == NULL)
    {
        return left_body;
    }
    if (strcmp("||", cpy->name) == 0 || strcmp("&&", cpy->name) == 0)
    {
        name = cpy;
        cpy = cpy->next;
        if (cpy == NULL)
        {
            return left_body;
        }
        while (strcmp("\n", cpy->name) == 0)
        {
            cpy = cpy->next;
            if (cpy == NULL)
            {
                return left_body;
            }
        }
        if ((right_body = and_or(&cpy)) == NULL)
            return left_body;
        else
        {
            *t = cpy;
            struct AST *node = operator_init(name);
            node->child[0] = left_body;
            node->child[1] = right_body;
            return node;
        }
    }
    return left_body;
}
