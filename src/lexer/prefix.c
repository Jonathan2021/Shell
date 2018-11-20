#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *prefix_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *prefix(struct Token **t)
{
    struct AST *redir;
    if (strcmp(t[0]->type,"ASSIGMENT_WORD") == 0)
    {
        struct AST *assigment_word = prefix_init(*t);
        *t = t[0]->next;
        return assigment_word;
    }
    else if ((redir = redirection(t)) != NULL)
    {
        return redir;
    }
    else
        return NULL;
}
