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

//Case_item on top and all words are its children, last child is cmp_list
struct AST *case_item(struct Token **t)
{
    struct AST *part1 = NULL;
    struct AST *part2 = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"(") == 0)
        tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    tmp = tmp->next;
    part1 = case_init();
    add_case(part1, word_init(tmp));
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
