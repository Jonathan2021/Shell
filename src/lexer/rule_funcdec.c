#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *func_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *funcdec(struct Token **t)
{
    struct AST *foo;
    struct Token *name;
    struct Token *tmp = *t;
    if (strcmp("function", tmp->name) == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    if (strcmp("WORD", tmp->type) == 0)
    {
        // a verifier avec regex
        name = tmp;
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    else
        return NULL;
    if (strcmp("(", tmp->name) != 0)
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL || strcmp(")", tmp->name) != 0)
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL)
        return NULL;

    while (strcmp("\n", tmp->name) == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    if ((foo = shell_command(&tmp)) != NULL)
    {
        *t = tmp;
        struct AST *node = func_init(name);
        node->child[0] = foo;
        return node;
    }
    return NULL;
}
