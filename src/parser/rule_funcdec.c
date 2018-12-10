#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"

void set_func(
    __attribute__((unused)) char *a, __attribute__((unused)) struct AST *b)
{
    return;
}

void foo_func(struct AST *node, __attribute__((unused)) struct fds fd)
{
    if (!node || node->nb_child < 2 || !node->child[0] || !node->child[1])
        return;
    set_func(node->child[0]->self->name, node->child[1]);
}

struct AST *func_init(void)
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    struct AST *node = AST_init(2);
    if (!node)
    {
        free(token);
        return NULL;
    }
    token->name = "function";
    token->type = "FUNCTION";
    node->self = token;
    node->foo = foo_func;
    return node;
}

struct AST *funcdec(struct Token **t)
{
    struct AST *body;
    struct Token *name;
    struct Token *tmp = *t;
    if (strcmp("function", tmp->name) == 0)
    {
        next_token(&tmp);
        if (tmp == NULL)
            return NULL;
    }
    if (strcmp("WORD", tmp->type) == 0)
    {
        next_token(&tmp);
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    else
        return NULL;
    if (strcmp("(", tmp->name) != 0)
        return NULL;
    next_token(&tmp);
    if (tmp == NULL || strcmp(")", tmp->name) != 0)
        return NULL;
    next_token(&tmp);
    if (tmp == NULL)
        return NULL;

    while (strcmp("\n", tmp->name) == 0)
    {
        next_token(&tmp);
        if (tmp == NULL)
            return NULL;
    }
    if ((body = shell_command(&tmp)) != NULL)
    {
        *t = tmp;
        struct AST *node = func_init();
        node->child[0] = word_init(name);
        node->child[1] = body;
        return node;
    }
    return NULL;
}
