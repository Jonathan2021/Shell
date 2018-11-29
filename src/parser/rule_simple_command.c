#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "include/foo.h"

int reserve(struct Token *token)
{
    if (!token)
        return 0;
    char *reserved[16][3] = 
    {{"IF", "if", "\0"},
    {"THEN", "then", "\0"},
    {"ELSE" , "else" , "\0"},
    {"ELIF", "elif", "\0"},
    {"FI", "fi", "\0"},
    {"DO", "do", "\0"},
    {"DONE", "done", "\0"},
    {"CASE", "case", "\0"},
    {"ESAC", "esac", "\0"},
    {"WHILE", "while", "\0"},
    {"UNTIL", "until", "\0"},
    {"FOR", "for", "\0"},
    {"LBRACE", "{", "\0"},
    {"Rbrace", "}", "\0"},
    {"BANG", "!", "\0"},
    {"IN", "in", "\0"}};
    for (int i = 0; i < 16; ++i)
    {
        if(!strcmp(reserved[i][1], token->name))
        {
            token->type = reserved[i][0];
            break;
        }
    }
    return 1;
}

struct AST *simple_cmd_init(void)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    struct Token *t = malloc(sizeof(struct Token));
    t->name = "simple command";
    t->type = "SIMPLE COMMAND";
    node->self = t;
    node->foo = foo_compound;
    return node;
}

struct AST *simple_command(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *list;
    struct AST *node;
    list = simple_cmd_init();
    if ((node = prefix(&tmp)) != NULL)
    {
        *t = tmp;
        if (tmp == NULL)
        {
            free_l(list);
            return node;
        }
        add_list(list, node);
        while ((node = prefix(&tmp)) != NULL)
        {
            *t = tmp;
            add_list(list, node);

            if (tmp == NULL)
                return list;
        }
    }
    if (reserve(tmp) && (node = element(&tmp)) != NULL)
    {

        *t = tmp;
        add_list(list, node);
        if (tmp == NULL)
            return list;
        while ((node = element(&tmp)) != NULL)
        {
            *t = tmp;
            add_list(list, node);
            if (tmp == NULL)
                return list;
        }
    }
    if (list->nb_child != 0)
        return list;
    free_l(list);
    return NULL;
}
