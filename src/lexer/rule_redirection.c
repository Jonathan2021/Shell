#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct AST *redirection_init(struct Token *token)
{
    struct AST *node = AST_init(2); //FIXME Dans la fn en dessous seul child[0] est set;
    if (!node)                      //FIXME et je pense qu il faut init 3 au cas ou y a un IOnumber
        return NULL;
    node->self = token;
    node->child[0] = AST_init(0);
    return node;
}

struct AST *redirection(struct Token **t)
{
    //struct Token *ionum;
    struct Token *pipe;
    struct Token *type;
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
    if (strcmp("IONUMBER", tmp->type) == 0) //FIXME On doit le retenir ca je pense, la tu sautes juste au prochain
    {
      //  ionum = tmp;
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
                node->child[0]->self = pipe; //FIXME AST_init set les child à nul donc risque de segfault

                *t = tmp;
                return node;
            }
        }    
    }//FIXME reternir list[i][1] aussi
    return NULL;
}
