#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
struct AST *case_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}
void add_case(struct AST *case_ast, struct AST *new)
{
    case_ast->nb_child++;
    case_ast->child = realloc(case_ast->child, \
    case_ast->nb_child * sizeof(struct AST *));
    case_ast->child[case_ast->nb_child -1] = new;
}

struct AST *case_item(struct Token **t)
{
    struct AST *part1 = NULL;
    struct AST *part2 = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"(") == 0)
        tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    part1 = case_init(tmp);
    while(strcmp(tmp->name,"|") == 0 ||
            strcmp(tmp->type,"WORD") == 0)
    {
        part2 = case_init(tmp);
        add_case(part1, part2);
        tmp = tmp->next;
        if (tmp == NULL)
        {
            AST_destroy(part1);
            return NULL;
        }
    }
    if (strcmp(tmp->name,")"))
        return NULL;
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
    if ((cpd_list = compound_list(&tmp)) != NULL)
    {
        tmp = tmp->next;
    }
    add_case(part1, cpd_list);
    *t = tmp;
    return part1;
}
