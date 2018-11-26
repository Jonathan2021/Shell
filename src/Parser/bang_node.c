#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

void foo_bang(struct AST *node, struct fds fd)
{
    if(node && node->child[0])
    {
        node->child[0]->foo(node->child[0], fd);
        node->res = node->child[0]->res;
    }
}

struct AST *bang_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}

