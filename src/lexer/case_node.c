#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"

struct AST *case_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self;
    return node;
}
struct AST *add_case(struct AST *case_ast, struct AST *new)
{
    case_ast->nb_child++;
    case_ast->child = realloc(case_ast->child, \
    case_ast * sizeof(struct AST *));
    case_ast->child[case_ast->child -1] = new;
}

struct AST *case_item(struct Token **t)
{
    struct AST *1_word = NULL;
    struct AST *2_word = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"(") == 0)
        tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    1_word = case_init(tmp);
    while(strcmp(tmp->name,"|") == 0 ||
            strcmp(tmp->type,"WORD") == 0)
    {
        2_word = case_init(tmp);
        add_case(1_word, 2_word);
        tmp = tmp->next;
        if (tmp == NULL)
        {
            AST_destroy(1_word);
            return NULL
        }
    }
    if (strcmp(tmp->name,")"))
        return NULL;
    *t = tmp;
    if (tmp == NULL)
    {
        AST_destroy(1_word);
        return NULL;
    }
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            *t = tmp;
            return 1_word;
        }
    }
    struct AST *cpd_list = NULL;
    if ((cpd_list = compound_list(&tmp)) != NULL)
    {
        tmp = tmp->next;
    }
    add_case(1_word, cpd_list);
    *t = tmp;
    return 1_word;
}
