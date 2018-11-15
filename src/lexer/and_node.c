#include <stdio.h>
#include <stdlib.h>
#include "my_tree.h"

struct AST *and_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

int foo_and(struct AST *node)
{
    if (node->child[0]->res && node->child[1]->res)
    {
        node->res = 1;
        return 1;
    }
    node->res = 0;
    return 0;
}
