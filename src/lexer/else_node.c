#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *else_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if(!node)
        return NULL;
    node->self = token;
    return node;
}
struct AST *elif_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

void foo_elif(struct AST *node, struct fds fd)
{
    if (!node || !node->child)
        return;
    node->child[0]->foo(node->child[0], fd);
    node->res = node->child[0]->res;
}

struct AST *else_clause(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *elif = NULL;
    if (strcmp(tmp->name, "else") == 0)
    {
        next_token(&tmp);
        if ((elif = compound_list(&tmp)) != NULL)
        {
            *t = tmp;
            return elif;
        }
    }
    tmp = *t;
    if (strcmp(tmp->name, "elif") == 0)
    {
        next_token(&tmp);
        struct AST *condition = NULL;
        struct AST *elif_body = NULL;
        struct AST *else_body = NULL;
        if ((condition = compound_list(&tmp)) == NULL)
            return NULL;
        if (tmp == NULL)
            call_ps2(t, &tmp);

        if (strcmp("then", tmp->name) != 0)
        {
            AST_destroy(condition);
            return NULL;
        }
        next_token(&tmp);

        if ((elif_body = compound_list(&tmp)) == NULL)
        {
            AST_destroy(condition);
            return NULL;
        }
        if (tmp == NULL)
            call_ps2(t, &tmp);

        else_body = else_clause(&tmp);
        *t = tmp;
        struct AST *elif = elif_init(*t);
        elif->child[0] = condition;
        elif->child[1] = elif_body;
        elif->child[2] = else_body;
        return elif;
    }
    else
        return NULL;
}
