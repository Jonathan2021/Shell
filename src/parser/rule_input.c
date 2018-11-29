/**
 ** \file parser/rule_input.c
 ** \brief the grammar rule for input (see subject)
 ** \date 29 novembre 2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

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
        if (tmp == NULL || strcmp(tmp->name, "\n") == 0)
        {
            *t = tmp;
            return node;
        }
        else
        {
            AST_destroy(node);
            return NULL;
        }
    }
    return NULL;
}
