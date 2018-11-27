#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"

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
    next_token(&tmp);
    if ((do_grp=compound_list(&tmp)) == NULL)
        return NULL;
    if (tmp == NULL)
        call_ps2(t, &tmp);
    if (strcmp(tmp->name,"done"))
    {
        AST_destroy(do_grp);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    return do_grp;
}
