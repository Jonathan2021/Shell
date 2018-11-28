#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"


struct AST *simple_cmd_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = "simple command";
    t->type = "SIMPLE COMMAND";
    node->self = t;
    node->foo = foo_compound;
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
                return list;
        }
    }
    if ((node = element(&tmp)) != NULL)
    {

        *t = tmp;
        add_list(list, node);
        if (tmp == NULL)
            return list;
        while ((node = element(&tmp)) != NULL)
        {
            *t = tmp;
            add_list(list, node);
            if (tmp == NULL)
                return list;
        }
    }
    if (list->nb_child != 0)
        return list;
    free_l(list);
    return NULL;
}
