#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"



struct AST *input(struct Token **t)
{
    struct AST *node;
    struct Token *tmp = *t;
    if (tmp == NULL || strcmp(tmp->name, "\n") == 0)
        return NULL;
    if ((node = list(&tmp)) != NULL)
    {
        if (tmp == NULL || strcmp(tmp->name,"\n") == 0)
        {
            *t = tmp;
            return node;
        }
        else
        {
            AST_destroy(node);
            return NULL;
        }
    }
    return NULL;
}
