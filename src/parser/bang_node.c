/**
 ** \file parser/bang_node.c
 ** \brief create bang node
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief execution of bang node
 ** \param node ast tree for execution
 ** \param fd  file descriptor for execution
 **/

void foo_bang(struct AST *node, struct fds fd)
{
    if(node && node->child[0])
    {
        node->child[0]->foo(node->child[0], fd);
        node->res = node->child[0]->res;
    }
}

/**
 ** \brief init bang node
 ** \param linked list of token
 ** \return node bang
 **/

struct AST *bang_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}

