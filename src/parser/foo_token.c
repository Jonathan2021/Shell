/**
 ** \file parser/foo_token.c
 ** \brief give next token to ast and ask for a PS2 when it's incomplete
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "../include/shell.h"
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief init else node
 ** \param token linked list
 ** \return node else
 **/

void next_token(struct Token **tmp)
{
    struct Token *res = tmp[0]->next;
    if (res == NULL || (strcmp("\n", res->name) == 0 && res->next == NULL))
    {
        char *str = malloc(4095); // a free plus tard
        fprintf(stderr, "%s", getvalue("$PS2"));
        if (fgets(str, 4095, stdin) == NULL)
            return;
        res = create_token(res, str);
        tmp[0]->next = res;
        free(str);
    }
    *tmp = res;
}

struct Token *call_ps2(struct Token **t)
{
    struct Token *tmp = *t;
    for (; tmp->next != NULL; tmp = tmp->next)
        ;
    struct Token *res = NULL;
    char *str = readline(getvalue("$PS2"));
    if (!str)
        return NULL;
    res = create_token(res, str);
    tmp->next = res;
    free(str);
    return res;
}
