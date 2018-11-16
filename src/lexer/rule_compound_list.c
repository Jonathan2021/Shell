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
    struct Token *cpy;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return 0;
    }
    if (!tmp || !(and_or_ast = and_or(&tmp)))
        return NULL;
    *t = tmp;
    if(!(compound = compound_init()))
    {
        AST_destroy(and_or_ast);
        return NULL;
    }
    add_compound(compound, and_or_ast);
    *t = tmp;
    while(1)
    {
        cpy = tmp;
        if (cpy && (!strcmp(cpy->name,";") ||
            !strcmp(cpy->name,"&") ||
            !strcmp(cpy->name,"\n")))
        {
            separator = word_init(cpy);
            cpy = cpy->next;
            while (cpy && !strcmp(cpy->name,"\n"))
                cpy = cpy->next;
            if (!cpy || !(and_or_ast = and_or(&cpy)))
                break;
            add_compound(compound, separator);
            add_compound(compound, and_or_ast);
            tmp = cpy;
            *t = tmp;
        }
    }
    if (tmp && (!strcmp(tmp->name,"&") ||
        !strcmp(tmp->name,";") ||
        !strcmp(tmp->name,"\n")))
    {
        add_compound(compound, word_init(tmp));
        tmp = tmp->next;
        while(tmp && !strcmp(tmp->name,"\n"))
        {
            tmp = tmp->next;
        }
        *t = tmp;
    }
    if (compound->nb_child == 1)
    {
        and_or_ast = compound->child[0];
        free_l(compound);
        return and_or_ast;
    }
    return compound;
}
