#include <stdio.h>
#include <stdlib.h>
#include "my_tree.h"

struct AST *if_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = condition
    //node->child[1] = ifbody
    //node->child[2] = else body
    return node;
}

int foo_if(struct AST *node)
{
    if (node->child[0]->res)
    {
        //go dans fils 1;
        node->res = 1;
        return 1;
    }
    //go dans fils 2;
    node->res = 0;
    return 0;
}
