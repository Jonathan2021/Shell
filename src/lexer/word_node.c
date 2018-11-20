nclude<stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"

    struct AST *word_init(struct Token *token)
{
    struct AST *node = AST_init(1);
    if (!node)
        return NULL;
    node->self = token;
    // node->child[0] = left_body
    // node->child[1] = right_body
    return node;
}

struct AST *shell_command(struct Token **t)
{
    struct Token *t1 = *t;
    struct Token *t2 = *t;
    struct Token *t3 = *t;
    struct AST *shell = NULL;
    if (strcmp("{", t1->name) == 0)
    {
        t1 = t1->next;
        if (t1 == NULL)
            return NULL;
        if ((shell = list(&t1)) != NULL)
        {
            if (t1 == NULL)
                return NULL;
            if (strcmp("}", t1->name) == 0)
            {
                if (t1 != NULL)
                    t1 = t1->next;
                t = &t1;
                return shell;
            }
        }
    }
    else if ((shell = rule_if(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    else if (strcmp("WORD", t3->type) == 0)
    {
        if (t3 != NULL)
            t3 = t3->next;
        *t = t3;
        shell = word_init(t);
        return shell;
    }
    return NULL;
}
