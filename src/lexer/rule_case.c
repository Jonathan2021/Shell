#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"



struct AST *rule_case(struct Token **t)
{
    struct AST *condition;
    struct AST *case_body = NULL;
    struct Token *name;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"case"))
        return NULL;
    name = tmp;
    tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    condition = case_init(tmp);
    tmp = tmp->next;
    if (tmp == NULL)
    if (tmp == NULL)
    {
        AST_destroy(condition);
        return NULL;
    }
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            break;
    }
    if (tmp == NULL || strcmp(tmp->name,"in"))
    {
            AST_destroy(condition);
            return NULL;
    }
    tmp = tmp->next;
    if (tmp == NULL)
    {
        AST_destroy(condition);
        return NULL;
    }
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            AST_destroy(condition);
            return NULL;
        }
    }
    if ((case_body = case_clause(&tmp)) != NULL)
        tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->name,"esac"))
    {
        if (case_body)
            AST_destroy(case_body);
        AST_destroy(condition);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = if_init(name); //FIXME create case_init because if_init has foo_if inside
    node->child[0] = condition;
    node->child[1] = case_body;
    return node;
}
