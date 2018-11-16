#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"


struct AST *input(struct Token **t)
{
    struct AST *node;
    struct Token *tmp = *t;
    if ((node = list(&tmp)) != NULL)
    {
        if (tmp == NULL)
        {
            *t = tmp;
            return node;
        }
        else if (strcmp(tmp->name, "\n") == 0)
        {
            *t = t[0]->next;
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
