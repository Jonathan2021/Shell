#include <stdlib.h>
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *compound_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    token->type = "COMPOUND";
    token->name = "compound";
    struct AST *node = AST_init(0);
    if(!node)
    {
        free(token);
        return NULL;
    }
    node->self = token;
    return node;
}

void add_compound(struct AST *compound, struct AST *new)
{
    compound->nb_child++;
    compound->child = realloc(compound->child, \
    compound->nb_child * sizeof(struct AST));
    compound->child[compound->nb_child-1] = new;
}

struct AST *compound_list(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *and_or_ast;
    struct AST *separator;
    struct AST *compound = NULL;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return 0;
    }
    *t = tmp;
    if (!tmp || !(and_or_ast = and_or(&tmp)))
        return NULL;
    if(!(compound = compound_init()))
    {
        free_ast(and_or_ast);
        return NULL;
    }
    add_compound(compound, and_or_ast);
    *t = tmp;
    while(1)
    {
        if (tmp && (!strcmp(tmp->name,";") ||
            !strcmp(tmp->name,"&") ||
            !strcmp(tmp->name,"\n")))
        {
            separator = tmp;
            tmp = tmp->next;
            while (tmp && !strcmp(tmp->name,"\n"))
                tmp = tmp->next;
            if (!tmp || !(and_or_ast = and_or(&tmp)))
                break;
            add_compound(compound, separator);
            add_compound(compound, and_or_ast);
            *t = tmp;
        }
    }
    if (tmp && (!strcmp(tmp->name,"&") ||
        !strcmp(tmp->name,";") ||
        !strcmp(tmp->name,"\n")))
        {
            add_compund(compound, tmp);
            tmp = tmp->next;
            while(tmp && !strcmp(tmp->name,"\n"))
            {
                tmp = tmp->next;
            }
            *t = tmp;
        }
    return compound;
}
