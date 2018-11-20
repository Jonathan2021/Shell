#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"



struct AST *while_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

void foo_while(struct AST *node)
{
    if (!node || !node->child[0])
        return;
    node->child[0]->foo(node->child[0]);
    node->res = node->child[0]->res;
}

struct AST *rule_while(struct Token **t)
{
    struct AST *condition;
    struct AST *do_body;
    struct Token *name;

    struct Token *tmp = *t;
    if (strcmp("while", tmp->name))
        return NULL;
    name = tmp;
    tmp = tmp->next;
    if (!tmp || !(condition = compound_list(&tmp)))
        return NULL;
    if (!tmp || !(do_body = do_group(&tmp)))
        return NULL;
    //Pas besoin  de tmp->next?
    struct AST *node = while_init(name);
    node->child[0] = condition;
    node->child[1] = do_body;
    *t = tmp;
    node->foo = foo_while;
    return node;
}
