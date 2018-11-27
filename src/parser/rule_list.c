#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"

/*
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
*/

struct AST *list_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = "list";
    t->type = "LIST";
    node->self = t;
    node->foo = foo_compound;
    return node;
}

struct AST *list(struct Token **t)
{ 
    struct AST *node = NULL;
    struct Token *t2 = *t;
    if ((node = and_or(&t2)) == NULL)
        return NULL;
    *t = t2;
    struct AST *list = list_init();
    add_list(list, node);
    while (t2 && (strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0))
    {
        add_list(list, word_init(t2));
        t2 = t2->next;
        *t = t2;
        if (!t2 || (node = and_or(&t2)) == NULL)
            break;
        add_list(list, node);
        *t = t2;
    }
    return list;
}
