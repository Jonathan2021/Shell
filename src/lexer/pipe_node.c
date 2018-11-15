#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"

struct AST *pipe_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}
