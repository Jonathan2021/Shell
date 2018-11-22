#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *rule_case_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if(!token)
        return NULL;
    struct AST *node = AST_init(2);
    if(!node)
    {
        free(token);
        return NULL;
    }
    token->type = "CASE";
    token->name = "case";
    node->self = token;
    return node;
}

struct AST *rule_case(struct Token **t)
{
    struct Token *condition;
    struct AST *case_body = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"case"))
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    condition = tmp;
    tmp = tmp->next;
    if (tmp == NULL)
        return NULL;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            break;
    }
    if (tmp == NULL || strcmp(tmp->name,"in"))
            return NULL;
    tmp = tmp->next;
    if (tmp == NULL)
        return NULL;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            return NULL;
        }
    }
    case_body = case_clause(&tmp);
    if (tmp == NULL || strcmp(tmp->name,"esac"))
    {
        if (case_body)
            AST_destroy(case_body);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = rule_case_init(); 
    node->child[0] = word_init(condition);
    node->child[1] = case_body;
    return node;
}
