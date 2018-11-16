#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

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

void add_case(struct AST *case_clause, struct AST *case_item)
{
    case_clause->nb_child++;
    case_clause->child = realloc(case_clause->child, \
    case_clause->nb_child*sizeof(struct AST));
    case_clause->child[case_clause->nb_child-1] = a;
}

struct AST *case_clause(struct Token **t)
{
    struct AST *origin = case_clause_init();
    if (!origin)
        return NULL;
    struct AST *cur_case_item;
    struct Token tmp = *t;

    if (!tmp || !(cur_case_item = case_item(&tmp)))
        return NULL;
    add_case(origin, cur_case_item);
    *t = tmp;
    while (1)
    {
        if(!tmp || strcmp(";;", tmp->name))
            break;
        else
            tmp->next;
        while(tmp && !strcmp("\n", tmp->name))
        {
            tmp = tmp->next;
        }
        if(!tmp || !(cur_case_item = case_item(&tmp)))
            break;
        add_case(origin, cur_case_item);
        *t = tmp;
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
