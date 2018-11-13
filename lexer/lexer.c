#include <stdio.h>
#include <string.h>

int input(struct Token *t)
{
    if (list(t) == 1)
    {
        if (t == NULL)
            return 1;
        else
            return 0;
    }
    else if (t == NULL)
    {
        return 1;
    }
    else
        return 0;
}

int list(struct Token *t)
{
    int check = 0;
    if (and_or(t) == 0)
        return 0;
    while (1)
    {
        check = 0;
        if (strcmp(";", t->name) == 0 || strcmp("&", t->name) == 0)
        {
            check = 1;
            t = t->next;
            if (and_or(t) == 0)
            {
                break;
            }
        }
        else
            break;
    }
    if (check == 0 && ((strcmp(";", t->name) == 0 || strcmp("&", t->name) == 0)))
        t = t->next;
    return 1;
}

int and_or(struct Token *t)
{
}

