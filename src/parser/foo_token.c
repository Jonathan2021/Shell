#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "../include/shell.h"

void next_token(struct Token **tmp)
{
    struct Token *res = tmp[0]->next;
    if (res == NULL || (strcmp("\n", res->name) == 0 && res->next == NULL))
    {
        char *str = malloc(4095); // a free plus tard
        printf("> ");
        if (fgets(str,4095,stdin) == NULL )
            return ;
        res = create_token(res,str);
        tmp[0]->next = res;
    }
    *tmp = res;
}

struct Token *call_ps2(struct Token **t)
{
    struct Token *tmp = *t;
    for (; tmp->next != NULL; tmp = tmp->next);
    char *str = malloc(4095); // a free plus tard
    printf("> ");
    struct Token *res = NULL;
    if (fgets(str,4095,stdin) == NULL )
        return NULL;
    res = create_token(res,str);
    tmp->next = res;
    return res;
}
