#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"


struct AST *do_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *do_group(struct Token **t)
{
    struct AST *do_grp;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"do"))
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL || (do_grp=compound_list(&tmp)) != NULL)
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->name,"done"))
    {
        AST_destroy(do_grp);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    return do_grp;
} 
