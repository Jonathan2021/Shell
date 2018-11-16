#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *operator_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

int foo_and(struct AST *node)
{
    if (node->child[0]->res && node->child[1]->res)
    {
        node->res = 1;
        return 1;
    }
    node->res = 0;
    return 0;
}

struct AST *and_or(struct Token **t)
{
    struct AST *left_body;
    struct AST *right_body;
    struct Token *name;

    struct Token *cpy = *t;
    //int check = 2;
    if ((left_body = pipeline(&cpy)) == NULL)
        return NULL;
    *t = cpy;
    if (cpy == NULL)
    {
        return left_body;
    }
    if (strcmp("||", cpy->name) == 0 || strcmp("&&", cpy->name) == 0)
    {
        name = cpy;
        cpy = cpy->next;
        if (cpy == NULL)
        {
            return left_body;
        }
        while (strcmp("\n", cpy->name) == 0)
        {
            cpy = cpy->next;
            if (cpy == NULL)
            {
                return left_body;
            }
        }
        if ((right_body = and_or(&cpy)) == NULL)
            return left_body;
        else
        {
            *t = cpy;
            struct AST *node = operator_init(name);
            node->child[0] = left_body;
            node->child[1] = right_body;
            return node;
        }
    }

    return left_body;
}
