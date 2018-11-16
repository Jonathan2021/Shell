#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"


struct AST *input_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if(!token)
        return NULL;
    struct AST *node = AST_init(1);
    if(!node)
    {
        free(token);
        return NULL;
    }
    token->type = "INPUT";
    token->name = "input";
    node->self = token;
    return node;
}
struct AST *input(struct Token **t)
{
    struct AST *node = input_init();
    struct Token *tmp = *t;
    node->child[0] = list(&tmp);
    if(!tmp || !strcmp(tmp->name, "\n")
        return node;
    AST_destroy(node);
    return NULL;
}

int foo_input(struct AST *node)
{
    if(!node)
        return 0;
    if(node->child[0])
        node->res = node->child[0]->res;
    else
        node->res = 1;
    return node->res;
}


