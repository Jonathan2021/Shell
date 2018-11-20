#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *pipe_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

/* struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct AST *tmp_ast;
    struct AST *rattach;
    struct AST *first_cmd;

    struct Token *tmp = *t;
    if (tmp && !strcmp("!", tmp->name))
    {
        origin = bang_init(tmp);
        tmp = tmp->next;
    }
    if (!tmp || !(rattach = command(&tmp)))
    {
        AST_destroy(origin);
        return NULL;
    }
    first_cmd = rattach;
    *t = tmp;
    while (1)
    {
        if (strcmp("|", tmp->name))
            break;
        tmp_ast = rattach;
        rattach = pipe_init(tmp);
        rattach->child[0] = tmp_ast;
        tmp = tmp->next;
        while (tmp && !strcmp("\n", tmp->name))
            tmp = tmp->next;
        if (!tmp || !(tmp_ast = command(&tmp)))
        {
            tmp_ast = rattach->child[0];
            AST_destroy(rattach);
            rattach = tmp_ast;
            break;
        }
        rattach->child[1] = tmp_ast;
        *t = tmp;
    }
    if (origin)
        origin->child[0] = rattach;
    else
        origin = rattach;
    return origin;
} */

struct AST *r_pipeline(struct Token **t)
{
    struct AST *left_child;
    struct AST *right_child;
    struct Token *name;

    struct Token *tmp = *t;
    if ((left_child = command(&tmp)) == NULL)
    {
        return NULL;
    }
    *t = tmp;
    if (tmp == NULL)
        return left_child;

    if (strcmp("|", tmp->name) != 0)
        return left_child;
    if (tmp == NULL)
        return left_child;
    name = tmp;
    tmp = tmp->next;

    while (tmp && !strcmp("\n", tmp->name))
        tmp = tmp->next;
    if (tmp == NULL)
        return left_child;

    if ((right_child = r_pipeline(&tmp)) == NULL)
    {
        return left_child;
    }
    struct AST *node = pipe_init(name);
    node->child[0] = left_child;
    node->child[1] = right_child;
    return node;
}
struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct Token *tmp = *t;
    if (tmp && !strcmp("!", tmp->name))
    {
        origin = bang_init(tmp);
        tmp = tmp->next;
        if ((origin->child[0] = r_pipeline(&tmp)) == NULL)
        {
            AST_destroy(origin);
            return NULL;
        }
        else
        {
            *t = tmp;
            return origin;
        }
    }
    if ((origin = r_pipeline(&tmp)) == NULL)
    {
        return NULL;
    }
    *t = tmp;
    return origin;
}
