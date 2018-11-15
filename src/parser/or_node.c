#include <stdio.h>
#include <stdlib.h>
#include "my_tree.h"

struct AST *or_init()
{
    struct AST *node = AST_init(2);
    node->name = "||";
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

int foo_or(struct AST *node)
{
    if (node->child[0]->res || node->child[1]->res)
    {
        node->res = 1;
        return 1;
    }
    node->res = 0;
    return 0;
}