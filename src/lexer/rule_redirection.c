#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *redirection_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    node->child[0] = AST_init(0);
    return node;
}

struct AST *redirection(struct Token **t)
{
    struct Token *ionum;
    struct Token *pipe;
    struct token *type;
    char *list[9][2] =
        {
            {">", "WORD"},
            {"<", "WORD"},
            {">>", "WORD"},
            {"<<", "HEREDOC"},
            {"<<-", "HEREDOC"},
            {">&", "WORD"},
            {"<&", "WORD"},
            {">|", "WORD"},
            {"<>", "WORD"},
        };
    struct Token *tmp = *t;
    if (strcmp("IONUMBER", tmp->type) == 0)
    {
        ionum = tmp;
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    struct Token *cpy = tmp;    
    for (int i = 0; i < 9; i++)
    {
        tmp = cpy;
        if (strcmp(list[i][0], tmp->name) == 0)
        {
            pipe = tmp;
            tmp = tmp->next;
            if (tmp == NULL)
                return NULL;
            if (strcmp(list[i][1], tmp->type) == 0)
            {
                type = tmp;
                struct AST *node = redirection_init(type);
                node[0]->self = pipe; 

                *t = tmp;
                return node;
            }
        }    
    }
    return NULL;
}