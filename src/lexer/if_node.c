#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *if_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = condition
    //node->child[1] = ifbody
    //node->child[2] = else body
    return node;
}

void foo_if(struct AST *node)
{
    if (!node || !node->child[0])
        return;
    node->child[0]->foo(node->child[0]);
    node->res = node->child[0]->res;
}

struct AST *rule_if(struct Token **t)
{
    struct AST *condition;
    struct AST *if_body;
    struct AST *else_body;
    struct Token *name;

    struct Token *tmp = *t;
    if (strcmp("if", tmp->name) != 0)
    {
        return NULL;
    }
    name = tmp;
    tmp = tmp->next;
    if (tmp == NULL || (condition = compound_list(&tmp)) == NULL)
    {
        return NULL;
    }
    if (tmp == NULL)
    {
        AST_destroy(condition);
        return NULL;
    }
    if (strcmp("then", tmp->name) != 0)
    {
        AST_destroy(condition);
        return NULL;
    }
    tmp = tmp->next;
    if (tmp == NULL || (if_body = compound_list(&tmp)) == NULL)
        return NULL;
    else_body = else_clause(&tmp);
    if (tmp == NULL || strcmp("fi", tmp->name) != 0)
    {
        AST_destroy(condition);
        AST_destroy(if_body);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = if_init(name);
    node->child[0] = condition;
    node->child[1] = if_body;
    node->child[2] = else_body;
    node->foo = foo_if;
    return node;
}
