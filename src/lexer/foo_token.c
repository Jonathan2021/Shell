#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct Token *get_Token(struct Token **tmp, struct Token **t)
{
    struct Token *res = tmp[0]->next;
    if (res == NULL || (strcmp("\n", res->name) == 0 && res->next == NULL))
    {
        char str[4095] = {0};
        printf("> ");
        fgets(str,4095,stdin);
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
    fgets(str,4095,stdin);
    res = create_token(res,str);
    tmp->next = res;
    *t2 = res;
}
