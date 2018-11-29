/**
 ** \file parser/case_node.c
 ** \brief case_node grammar and create node "case"
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
struct AST *case_init(void)
{
    struct Token *token = malloc(sizeof(struct Token));
    if(!token)
        return NULL;
    struct AST *node = AST_init(0);
    if (!node)
    {
        free(token);
        return NULL;
    }
    token->name = "case item";
    token->type = "CASE ITEM";
    node->self = token;
    return node;
}

/**
 ** \brief check case_item grammar and create node with token given in parameter 
 ** \param linked list of token
 ** \return node case_item
 **/

struct AST *case_item(struct Token **t)
{
    struct AST *part1 = NULL;
    struct AST *part2 = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"(") == 0)
        tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    part1 = case_init();
    add_case(part1, word_init(tmp));
    tmp = tmp->next;
    struct Token *cpy = tmp;
    while(strcmp(cpy->name,"|") == 0)
    {
        cpy = cpy->next;
        if (strcmp(cpy->type,"WORD") != 0)
            break;
        part2 = word_init(cpy);
        add_case(part1, part2);
        cpy = cpy->next;
        if (cpy == NULL)
        {
            AST_destroy(part1);
            return NULL;
        }
        tmp = cpy;
    }
    if (strcmp(tmp->name,")"))
        return NULL;
    tmp = tmp->next;
    *t = tmp;
    if (tmp == NULL)
    {
        AST_destroy(part1);
        return NULL;
    }
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            *t = tmp;
            return part1;
        }
    }
    struct AST *cpd_list = NULL;
    cpd_list = compound_list(&tmp);
    add_case(part1, cpd_list);
    *t = tmp;
    return part1;
}
