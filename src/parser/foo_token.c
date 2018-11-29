/**
 ** \file parser/foo_token.c
 ** \brief give next token to ast and ask for a PS2 when it's incomplete
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "../include/shell.h"

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
        printf("%s",get_file("PS2"));
        if (fgets(str,4095,stdin) == NULL )
            return ;
        res = create_token(res,str);
        tmp[0]->next = res;
        free(str);
    }
    *tmp = res;
}

struct Token *call_ps2(struct Token **t)
{
    struct Token *tmp = *t;
    for (; tmp->next != NULL; tmp = tmp->next);
    char *str = malloc(4095); // a free plus tard
    printf("%s",get_file("PS2"));
    struct Token *res = NULL;
    if (fgets(str,4095,stdin) == NULL )
        return NULL;
    res = create_token(res,str);
    tmp->next = res;
    free(str);
    return res;
}
