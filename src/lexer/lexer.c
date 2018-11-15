#include <stdio.h>
#include <string.h>
#include "include/lexer_struct.h"

int input(struct Token **t)
{
    struct Token *tmp = *t;
    if (list(&tmp) == 1)
    {
        if (tmp == NULL)
        {
            *t = tmp;
            return 1;
        }
        else
            return 0;
    }
    tmp = *t;
    if (tmp == NULL)
    {
        *t = tmp;
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
    if (t2 == NULL)
    {
        *t = t2;
        return 1;
    }
    while (1)
    {
        check = 0;
        if (strcmp(";", t2->name) == 0 || strcmp("&", t2->name) == 0)
        {
            check = 1;
            t2 = t2->next;
            if (t2 == NULL)
                return 1;
            if (and_or(&t2) == 0)
            {
                break;
            }
            if (t2 == NULL)
            {
                return 1;
            }
        }
        else
            break;
    }
    if (t2 == NULL)
    {
        *t = t2;
        return 1;
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
    if (cpy == NULL)
    {
        *t = cpy;
        return 1;
    }
    while (1)
    {
        if (check == 1)
            *t = cpy;
        if (check == 0)
        {
            *t = cpy;
            return 1;
        }
        if (strcmp("||", cpy->name) == 0 || strcmp("&&", cpy->name) == 0)
        {
            cpy = cpy->next;
            if (cpy == NULL)
            {
                return 1;
            }
            while (1)
            {

                if ((strcmp("\n", cpy->name) == 0))
                {
                    cpy = cpy->next;
                    if (cpy == NULL)
                    {
                        return 1;
                    }
                }
                else
                    break;
            }
            if (shell_command(&cpy) == 0)
                check = 1;
            else
            {
                check = 0;
                if (cpy == NULL)
                {
                    *t = cpy;
                    return 1;
                }
            }
        }
        else
            break;
    }
    return 1;
}
int rule_if(struct Token **t)
{
    struct Token *tmp = *t;
    if (strcmp("if", tmp->name) != 0)
    {
        return 0;
    }
    tmp = tmp->next;
    if (tmp == NULL || list(&tmp) == 0)
    {
        return 0;
    }
    if (tmp == NULL)
        return 0;
    if (strcmp("then", tmp->name) != 0)
    {
        return 0;
    }
    tmp = tmp->next;
    if (tmp == NULL || list(&tmp) == 0)
        return 0;
    else_clause(&tmp);
    if (tmp == NULL || strcmp("fi", tmp->name) != 0)
    {
        return 0;
    }
    tmp = tmp->next;
    *t = tmp;
    return 1;
}

int else_clause(struct Token **t)
{
    struct Token *tmp = *t;
    if (strcmp(tmp->name, "else") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            return 0;
        }
        if (list(&tmp) == 1)
        {
            *t = tmp;
            return 1;
        }
    }
    tmp = *t;
    if (strcmp(tmp->name, "elif") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL || list(&tmp) == 0)
            return 0;

        if (tmp == NULL || strcmp("then", tmp->name) != 0)
            return 0;
        tmp = tmp->next;

        if (tmp == NULL || list(&tmp) == 0)
            return 0;

        else_clause(&tmp);
        *t = tmp;
        return 1;
    }
    else
        return 0;
}
int shell_command(struct Token **t)
{
    struct Token *t1 = *t;
    struct Token *t2 = *t;
    struct Token *t3 = *t;
    if (strcmp("{", t1->name) == 0)
    {
        t1 = t1->next;
        if (t1 == NULL)
            return 0;
        if (list(&t1) == 1)
        {
            if (t1 == NULL)
                return 0;
            if (strcmp("}", t1->name) == 0)
            {
                if (t1 != NULL)
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
        if (t3 != NULL)
            t3 = t3->next;
        *t = t3;
        return 1;
    }
    return 0;
}

// nouvelle partie
int pipeline(struct Token **t)
{
    struct Token *tmp = *t;
    if (strcmp("!", tmp->name) == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return 0;
    }
    if (command(&tmp) == 0)
        return 0;
    while (1)
    {
        *t = tmp;
        if (strcmp("|", tmp->name) == 0)
        {
            tmp = tmp->next;
            if (tmp == NULL)
                return 1;
            while (strcmp("\n", tmp->name) == 0)
            {
                tmp = tmp->next;
                if (tmp == NULL)
                    return 1;
                }
            if (command(&tmp) == 0)
                return 0;
            if (tmp == NULL)
            {
                *t = tmp;
                return 1;
            }
        }
        else
            break;
    }
    return 1;
}

int command(struct Token **t)
{
    struct Token *tmp = *t;
    if (simple_command(&tmp) == 1)
    {
        *t = tmp;
        return 1;
    }
    tmp = *t;
    if (shell_command(&tmp) == 1)
    {
        *t = tmp;
        if (tmp == NULL)
            return 1;
        while (1)
        {
            if (redirection(&tmp) == 1)
                *t = tmp;
            else
                return 1;
            if (tmp == NULL)
                return 1;
        }
    }
    tmp = *t;
    if (funcdec(&tmp) == 1)
    {
        *t = tmp;
        if (tmp == NULL)
            return 1;
        while (1)
        {
            if (redirection(&tmp) == 1)
                *t = tmp;
            else
                return 1;
            if (tmp == NULL)
                return 1;
        }
    }
    return 0;
}

int simple_command(struct Token **t)
{
    struct Token *tmp = *t;
    if (prefix(&tmp) == 1)
    {
        *t = tmp;
        if (tmp == NULL)
            return 1;
        while (1)
        {
            if (prefix(&tmp) == 1)
                *t = tmp;
            else
                return 1;
            if (tmp == NULL)
                return 1;
        }
    }
    tmp = *t;
    while (1)
    {
        if (prefix(&tmp) == 1)
            *t = tmp;
        else
            break;
        if (tmp == NULL)
            return 0;
    }
    if (element(&tmp) == 1)
    {
        *t = tmp;
        if (tmp == NULL)
            return 1;
        while (1)
        {
            if (element(&tmp) == 1)
                *t = tmp;
            else
                return 1;
            if (tmp == NULL)
                return 1;
        }
    }
    return 0;
}


