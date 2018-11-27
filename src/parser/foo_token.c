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
        char str[4095] = {0};
        printf("> ");
        if (fgets(str,4095,stdin) == NULL )
            return ;
        res = create_token(res,str);
        tmp[0]->next = res;
    }
    *tmp = res;
}

void call_ps2(struct Token **t, struct Token **t2)
{
    struct Token *tmp = *t;
    for (; tmp->next; tmp = tmp->next);
    char str[4095] = {0};
    printf("> ");
    struct Token *res = NULL;
    if (fgets(str,4095,stdin) == NULL )
        return ;
    res = create_token(res,str);
    tmp->next = res;
    *t2 = res;
}
