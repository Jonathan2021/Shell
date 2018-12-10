/**
 ** \file parser/rule_input.c
 ** \brief the grammar rule for input (see subject)
 ** \date 29 novembre 2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

void next_newline(struct Token **t)
{
    struct Token *tmp = *t;
    while (tmp)
    {
        if (strcmp(tmp->name, "\n") == 0)
        {
            tmp = tmp->next;
            break;
        }
        tmp = tmp->next;
    }
    *t = tmp;
}

/**
 ** \brief the grammar rule which return the correct ast.
 ** \param t the token chain list
 ** \returm the correspondant AST of the givem chain list token
 **/
struct AST *input(struct Token **t)
{
    struct AST *node;
    struct Token *tmp = *t;
    if (tmp == NULL || strcmp(tmp->name, "\n") == 0)
        return NULL;
    if ((node = list(&tmp)) != NULL)
    {
        if (strcmp(tmp->name, "\n") == 0 || !tmp)
        {
            if (tmp)
                tmp = tmp->next;
            *t = tmp;
            return node;
        }
        else
        {
            set_status(2);
            next_newline(&tmp);
            *t = tmp;
            AST_destroy(node);
            return NULL;
        }
    }
    next_newline(t);
    return NULL;
}
