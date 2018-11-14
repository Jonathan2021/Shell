#include <stdio.h>
#include <string.h>

int input(struct Token **t)
{
    if (list(*t) == 1)
    {
        if (*t == NULL)
            return 1;
        else
            return 0;
    }
    else if (*t == NULL)
    {
        return 1;
    }
    else
        return 0;
}

int list(struct Token **t)
{
    int check = 0;
    struct Token *t2 = *t;
    if (and_or(&t2) == 0)
        return 0;
    while (1)
    {
        check = 0;
        if (strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)
        {
            check = 1;
            t2 = t2->next;
            if (and_or(&t2) == 0)
            {
                break;
            }
        }
        else
            break;
    }
    if (check == 0 && ((strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)))
        t2 = t2->next;
    *t = t2;
    return 1;
}

int and_or(struct Token **t)
{
    struct Token *cpy = *t;
    int check = 1;
    if (shell_command(&cpy) == 0)
        return 0;
    while(1)
    {
         if (check == 1)
             *t = cpy;
         if (check == 0)
             return 1;
        if (strcmp(";", cpy->name) == 0 || strcmp("&", cpy->name) == 0)
        {
            cpy = cpy->next;
            while (1)
            {

                if ((strcmp("\n", cpy->name) == 0))
                {
                    cpy = cpy->next;
                }
                else
                    break;
            }
            if (shell_command(&cpy) == 0)
                check = 1;
            else
                check = 0;
        }
        else
            break;
    }
    return 1;
}
int rule_if (struct Token **t)
{
    struct Token **tmp = t;
    if (strcmp("if", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
    *t = *t->next;
    if (list(t) == 0)
    {
        t = tmp;
        return 0;
    }
    *t = *t->next;
    if (strcmp("then", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
    else_clause(t);
    *t = *t->next;
    if (strcmp("fi", t->name) != 0)
    {
        t = tmp;
        return 0;
    }
    return 1;
}

int else_clause(struct Token **t)
{
    struct Token **tmp = t;
    if (strcmp(t->name, "else") == 0)
    {
        if (list(t) == 0)
        {
            t = tmp;
        }
        else
        {
            *t = *t->next;
            return 1;
        }
    }
    else
    {
        t = tmp;
        if (strcmp(t->name, "elif") == 0)
        {
            if (list(t) == 0)
                return 0;
            *t = *t->next;
            if (strcmp("then",t->name) != 0)
            {
                t = tmp;
                return 0;
            }
            *t = *t->next;
            if (list(t) == 0)
            {
                t = tmp;
                return 0;
            }
            *t = *t->next;
            else_clause(t);
            return 1;

        }
        else
            return 0;
    }
}   
int shell_command(struct Token **t)
{
    struct Token *t1 = *t;
    struct Token *t2 = *t;
    struct Token *t3 = *t;
    if (strcmp("{", t1->name) == 0)
    {
        t1 = t1->next;
        if (list(&t1) == 1)
        {
            if (strcmp("}", t1->name) == 0)
            {
                t1 = t1->next;
                t = &t1;
                return 1;
            }
        }
    }
    else if (rule_if(&t2) == 1)
    {
        *t = t2;
        return 1;
    }
    else if (strcmp("WORD", t3->type) == 0)
    {
        t3 = t3->next;
        *t = t3;
        return 1;
    }
    else
    {
        return 0;
    }
}
