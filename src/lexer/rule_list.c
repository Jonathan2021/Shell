#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *list_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = "list";
    t->type = "LIST";
    node->self = t;
    return node;
}

void add_list(struct AST *l, struct AST *a)
{
    l->nb_child++;
    l->child = realloc(l->child, l->nb_child * sizeof(struct AST *));
    l->child[l->nb_child - 1] = a;
}

struct AST *list(struct Token **t)
{
    struct AST *node = NULL;

    int check = 0;
    struct Token *t2 = *t;
    if ((node = and_or(&t2)) == 0)
        return NULL;
    *t = t2;
    struct AST *list = list_init();
    add_list(list, node);
    if (t2 == NULL)
    {
        // free list;
        return list->child[0];
    }
    while (1)
    {
        check = 0;
        if (strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)
        {
            check = 1;
            t2 = t2->next;
            if (t2 == NULL)
            {
                if (list->nb_child == 1)
                {
                    return list->child[0];
                }
                return list;
            }
            if ((node = and_or(&t2)) == 0)
            {
                break;
            }
            if (t2 == NULL)
            {
                add_list(list, node);
                *t = t2;
                if (list->nb_child == 1)
                {
                    return list->child[0];
                }
                return list;
            }
        }
        else
            break;
    }
    if (t2 == NULL)
    {
        *t = t2;
        if (list->nb_child == 1)
        {
            return list->child[0];
        }
        return list;
    }
    if (check == 0 && ((strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)))
    {
        t2 = t2->next;
    }
    if (list->nb_child == 1)
    {
        return list->child[0];
    }
    *t = t2;
    return list;
}
