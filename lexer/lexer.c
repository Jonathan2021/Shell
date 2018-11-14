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
int rule_if (struct Token *t)
{
    struct Token *tmp = t;
    if (strcmp("if", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
    t = t->next;
    if (list(t) == 0)
    {
        t = tmp;
        return 0;
    }
    t = t->next;
    if (strcmp("then", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
    else_clause(t);
    t = t->next;
    if (strcmp("fi", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
}

int else_clause(struct Token *t)
{
    if (strcmp(t->name, "else") == 0)
    {
        if (list(t) == 0)
            return 0;
        return 1;
    }
    else
    {
        if (strcmp(t->name, "elif") == 0)
        {
            if (list(t) == 0)
                return 0;
            if (strcmp("then",t->name) != 0)
                return 0;
            if (list(t) == 0)
                return 0;
            else_clause(t);
            return 1;

        }
        else
            return 0;
    }

}
