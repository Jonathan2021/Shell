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

int foo_if(struct AST *node)
{
    if (node->child[0]->res)
    {
        //go dans fils 1;
        node->res = 1;
        return 1;
    }
    //go dans fils 2;
    node->res = 0;
    return 0;
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
    if (tmp == NULL || (condition = list(&tmp)) == 0)
    {
        return NULL;
    }
    if (tmp == NULL)
        return NULL;
    if (strcmp("then", tmp->name) != 0)
    {
        return 0;
    }
    tmp = tmp->next;
    if (tmp == NULL || (if_body = list(&tmp)) == 0)
        return NULL;
    else_body = else_clause(&tmp);
    if (tmp == NULL || strcmp("fi", tmp->name) != 0)
    {
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = if_init(name);
    node->child[0] = condition;
    node->child[1] = if_body;
    node->child[2] = else_body;
    return node;
}