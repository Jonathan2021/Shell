/**
 ** \file parser/rule_list.c
 ** \brief Grammar rule for the list (see subject)
 ** \date 29 novembre 2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief Function which init a list AST node
 ** \return Node AST list
 **/
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
/**
 ** \brief Grammar function which create and fill the AST list node with the
 *good grammar rule
 ** \param t The chain list of tokens
 **/
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
        while (t2 && (strcmp("\n", t2->name) == 0))
        {
            t2 = t2->next;
            *t = t2;
        }
        if (!t2 || (node = and_or(&t2)) == NULL)
            break;
        add_list(list, node);
        *t = t2;
    }
    return list;
}
