/**
 ** \file parser/rule_case_clause.c
 ** \brief all about the case_clause say in the subject
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include <stdio.h>
/**
 ** \brief Initialization of the case_clause node
 ** \return The node case_clause but not with his good child
 **/
struct AST *case_clause_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    token->type = "CASE_CLAUSE";
    token->name = "case_clause";
    struct AST *node = AST_init(0);
    if (!node)
    {
        free(token);
        return NULL;
    }
    node->self = token;
    return node;
}
/**
 ** \brief add a child to the case node
 ** \param case_clause the case_clause
 ** \param add the case_item in the child of case_clause
 **/
void add_case(struct AST *case_clause, struct AST *case_item)
{
    case_clause->nb_child++;
    case_clause->child = realloc(case_clause->child, \
    case_clause->nb_child*sizeof(struct AST));
    case_clause->child[case_clause->nb_child-1] = case_item;
}
/**
 ** \brief Create the case_clause node with his child
 ** \param t the chain list of tokens
 ** \return The node case_clause with his good child
 **/
struct AST *case_clause(struct Token **t)
{
    struct AST *origin = case_clause_init();
    if (!origin)
        return NULL;
    struct AST *cur_case_item;
    struct Token *tmp = *t;

    if  (tmp == NULL || (cur_case_item = case_item(&tmp)) == NULL)
        return NULL;
    add_case(origin, cur_case_item);
    *t = tmp;
    struct Token *cpy = tmp;
    while (1)
    {
        if (!cpy || strcmp(";;", cpy->name))
            break;
        else
            cpy = cpy->next;
        while(cpy && !strcmp("\n", cpy->name))
        {
            cpy = cpy->next;
        }
        if(!cpy || !(cur_case_item = case_item(&cpy)))
            break;
        add_case(origin, cur_case_item);
        *t = cpy;
        tmp = cpy;
    }
    if(tmp && !strcmp(";;", tmp->name))
    {
        tmp = tmp->next;
        *t = tmp;
    }
    while(tmp && !strcmp("\n", tmp->name))
    {
        tmp = tmp->next;
        *t = tmp;
    }
    return origin;
}
