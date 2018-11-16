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


struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct AST *tmp_ast;
    struct AST *rattach;
    struct AST *first_cmd;

    struct token *tmp = *t;
    if (tmp && !strcmp("!", tmp->name))
    {
        origin = bang_init(tmp);
        tmp = tmp->next;
    }
    if (!tmp || !(rattach = command(&tmp)))
    {
        free_ast(origin);
        return NULL;
    }
    first_cmd = rattach;
    *t = tmp;
    while (1)
    {
        if(strcmp("|", tmp->name))
            break;
        tmp_ast = rattach;
        rattach = pipe_init(tmp);
        rattach->child[0] = tmp_ast;
        tmp = tmp->next;
        while(tmp && !strcmp("\n", tmp->name))
            tmp = tmp->next;
        if(!tmp || !(tmp_ast = command(&tmp)))
        {
            tmp_ast = rattach->child[0];
            free_node(rattach);
            rattach = tmp_ast;
            break;
        }
        rattach->child[1] = tmp_ast;
        *t = tmp;
    }
    if (origin)
        origin->child[0] = rattach;
    else
       origin = rattach; 
    return origin;
}
