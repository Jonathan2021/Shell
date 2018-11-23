#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

struct Token *get_Token(struct Token **tmp, struct Token **t)
{
    struct Token *res = *tmp;
    if (res == NULL)
    {
        char str[4095] = {0};
        printf("> ");                                                   
        char *check = fgets(str,4095,stdin);                                                                                                                            
        res = create_token(token,str);
        struct t2 = *t;
        for (; t2->next; t2 = t2->next);
        t2->next = token;
    } 
    else
    {
        *t = t[0]->next;
    }
    return res;
}