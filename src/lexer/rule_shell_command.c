#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"




struct AST *word_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

struct AST *shell_command(struct Token **t)
{
    struct Token *t1 = *t;
    struct Token *t2 = *t;
    struct AST *shell = NULL;
    if (strcmp("{", t1->name) == 0 || strcmp("(", t1->name) == 0)
    {
        t1 = t1->next;
        if (t1 == NULL)
            return NULL;
        if ((shell = list(&t1)) != NULL)
        {
            if (t1 == NULL)
            {
                AST_destroy(shell);
                return NULL;
            }
            if (strcmp("}", t1->name) == 0 || strcmp(")", t1->name) == 0)
            {
                if (t1 != NULL)
                    t1 = t1->next;
                t = &t1;
                return shell;
            }
            else
                AST_destroy(shell);
        }
    }
    else if ((shell = rule_for(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    else if ((shell = rule_while(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    else if ((shell = rule_until(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    else if ((shell = rule_case(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    else if ((shell = rule_if(&t2)) != NULL)
    {
        *t = t2;
        return shell;
    }
    return NULL;
}
