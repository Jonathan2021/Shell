#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"



struct AST *word_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

