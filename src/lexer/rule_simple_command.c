#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *simple_cmd_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = " ";
    t->type = " ";
    node->self = t;
    return node;
}

struct AST *simple_command(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *list;
    struct AST *node;
    list = simple_cmd_init();
    if ((node = prefix(&tmp)) != NULL)
    {
        *t = tmp;
        if (tmp == NULL)
        {
            free_l(list);
            return node;
        }
        add_list(list, node);
        while ((node = prefix(&tmp)) != NULL)
        {
            *t = tmp;
            add_list(list, node);

            if (tmp == NULL)
            {
                if (list->nb_child == 1)
                {
                    node = list->child[0];
                    free_l(list);
                    return node;
                }
                else
                {
                    return list;
                }
            }
        }
    }
    if ((node = element(&tmp)) != NULL)
    {

        *t = tmp;
        add_list(list, node);
        if (tmp == NULL)
        {
            if (list->nb_child == 1)
            {
                node = list->child[0];
                free_l(list);
                return node;
            }
            else
            {
                return list;
            }
        }
        while ((node = element(&tmp)) != NULL)
        {
            *t = tmp;
            add_list(list, node);
            if (tmp == NULL)
            {
                if (list->nb_child == 1)
                {
                    node = list->child[0];
                    free_l(list);
                    return node;
                }
                else
                {
                    return list;
                }
            }
        }
    }
    if (list->nb_child != 0)
    {
        if (list->nb_child == 1)
        {
            node = list->child[0];
            free_l(list);
            return node;
        }
        else
        {
            return list;
        }
    }
    free_l(list);
    return NULL;
}
