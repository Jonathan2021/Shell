#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *element_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *element(struct Token **t)
{
    struct AST *word;
    struct AST *redir;
    if (strcmp(t[0]->type,"WORD") == 0)
    {
        struct AST *word = element_init(t);
        *t = t[0]->next;
        return word;
    }
    else if ((redir = redirection(t)) != NULL)
    {
        return redir;
    }
    else
        return NULL;
}
