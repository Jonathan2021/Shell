#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"

struct AST *else_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}
