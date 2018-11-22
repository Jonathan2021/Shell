#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

void foo_list(struct AST *node, struct fds fd)
{
    if(!node || ! node->child[0])
        return;
    int index = 0;
    while (index < node->nb_child && node->child[index])
    {
        if (!strcmp(node->child[index]->self->name, ";")
            || !strcmp(node->child[index]->self->name, "&"))
        {
            index++;
            continue;
        }
        node->child[index]->foo(node->child[index], fd);
        index++;
    }
}


struct AST *list_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = "list";
    t->type = "LIST";
    node->self = t;
    node->foo = foo_list;
    return node;
}

struct AST *list(struct Token **t)
{ 
    struct AST *node = NULL;
    int check = 0;
    struct Token *t2 = *t;
    if ((node = and_or(&t2)) == NULL)
        return NULL;
    *t = t2;
    if (t2 == NULL)
    {
        return node;
    }
    struct AST *list = list_init();
    add_list(list, node);
    while (1)
    {
        check = 0;
        if (strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)
        {
            check = 1;
            add_list(list, word_init(t2));
            t2 = t2->next;
            if (t2 == NULL)
            {
                if (list->nb_child == 1)
                {
                    node = list->child[0];
                    free_l(list);
                    return node;
                }
                return list;
            }
            if ((node = and_or(&t2)) == NULL)
            {
                AST_destroy(node);
                break;
            }
            if (t2 == NULL)
            {
                add_list(list, node);
                *t = t2;
                if (list->nb_child == 1)
                {
                    node = list->child[0];
                    free_l(list);
                    return node;
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
            node = list->child[0];
            free_l(list);
            return node;
        }
        return list;
    }
    if (check == 0 && ((strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)))
    {
        add_list(list, word_init(t2));
        t2 = t2->next;
    }
    *t = t2;
    if (list->nb_child == 1)
    {
        node = list->child[0];
        free_l(list);
        return node;
    }
    return list;
}
