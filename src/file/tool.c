/**
 ** \file file/tool.c
 ** \brief Functions for cutting the user input
 ** \date 29 novembre 2018
 **/
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/shell.h"
#include "../built-in/include/built-in.h"

/**
 ** \brief our strncpy
 ** \param dest is the string destination
 ** \param src is the string source
 ** \param n size of the string to be copy
 ** \return The pointer on the destination string
 **/

char *my_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}

/**
 ** \brief Give the option value
 ** \param name is the option enter by the user
 ** \return NULL if the option doesn't exist or the correspondant value
 *associated to the option.
 **/
char *get_value(char *name)
{
    struct PS *tmp = ps;
    while (tmp->name)
    {
        if (strcmp(tmp->name, name) == 0)
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }
    return NULL;
}

/**
 ** \brief Give the value of the variable
 ** \param name is a basic string
 ** \return Yhe correspondant value associated to the variable.
 **/
char *getvalue(char *name)
{
    if (name[0] == '$')
        name = getenv(name + 1);
    return name;
}

/**
 ** \brief Set the value of the name pass in parameter
 ** \param name is the option
 ** \param value is the value which gone a be associated to the option
 **/
void setvalue(char *name, char *value)
{
    setenv(name, value, 1);
}

/**
 ** \brief Set the value of the name pass in parameter
 ** \param name is the variable
 ** \param value is the value which gone a be associated to the variable
 **/
void set_value(char *name, char *value)
{
    struct PS *tmp = ps;
    while (tmp->name)
    {
        if (strcmp(tmp->name, name) == 0)
        {
            tmp->value = value;
            return;
        }
        tmp = tmp->next;
    }
    tmp->value = value;
    tmp->name = name;
    tmp->next = get_ps();
}
/**
 ** \brief Free the struct ps
 **/
void reset_value(void)
{
    struct PS *tmp = ps;
    while (ps->name)
    {
        tmp = ps;
        ps = ps->next;
        free(tmp);
    }
    free(ps);
}

int end_string(char *parse)
{
    if (parse && (strncmp(parse, "\"", 1) == 0 || strncmp(parse, "\'", 1) == 0)
        && strncmp(parse - 1, "\\", 1) != 0)
        return 1;
    return 0;
}

static int my_strlen(char *str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

char *create_word(char **parse, char **str2)
{
    char *cpy = calloc(4096, 1);
    char *check = *parse;
    int len = my_strlen(*str2) - 1;
    strncat(cpy, *parse, 1);
    *parse = *parse + 1;
    while (len != 0 && *parse && !end_string(*parse))
    {
        if (*parse[0] == '\0')
            strcat(cpy, " ");
        else
            strncat(cpy, *parse, 1);
        *parse = *parse + 1;
        len--;
    }
    if (len == 0)
        *parse = NULL;
    else if (*parse)
    {
        strncat(cpy, *parse, 1);
        *parse = *parse + 1;
    }
    *parse = check;
    cpy[my_strlen(cpy)] = '\0';
    return cpy;
}

static int check_quote(char *str)
{
    for (int i = 0; i < my_strlen(str); i++)
    {
        if (i > 0 && str[i - 1] == '\\' && (str[i] == '\"' || str[i] == '\''))
            continue;
        if (str[i] == '\"' || str[i] == '\'')
            return 1;
    }
    return 0;
}

char *parse_quote(char **parse, char **pt)
{
    char *delim = {"\t \n"};
    char *cpy = create_word(parse, pt);
    if (*parse[0] == parse[0][strlen(*parse) - 1]
        && ((*parse[0] == '\"' && strlen(*parse) > 1)
               || (*parse[0] == '\'' && strlen(*parse) > 1)))
    {
        *parse = strtok(NULL, delim);
        return cpy;
        ;
    }
    *parse = strtok(NULL, delim);
    while (*parse && check_quote(*parse) == 0)
    {
        *parse = strtok(NULL, delim);
    }
    if (*parse && check_quote(*parse) == 1)
        *parse = strtok(NULL, delim);
    return cpy;
}

/**
 ** \brief create the chain list composed of token.
 ** \param token list chain
 ** \param str the full argument of -c
 ** \return The new chain list of token.
 **/
struct Token *create_token(struct Token *token, char *str)
{
    char *parse;
    char *delim = {"\t \n"};
    char *str2 = calloc(40960, 1);
    char *pt = str2;
    strcpy(str2, str);
    parse = strtok(str, delim);
    char *grammar[20] = {")", "(", "&&", "||", ";;", "<<", ">>", "<&", ">&",
        "<>", "<<-", ">|", ";", "&", ">", "<"};
    while (parse)
    {
        for (int j = 0; parse && parse[j] != '\0';)
        {
            for (int i = 0; grammar[i]; i++)
            {
                if (is_ionumber(parse + j)
                    || strncmp(parse + j, grammar[i], strlen(grammar[i])) == 0)
                {
                    if (is_ionumber(parse + j) || j != 0)
                    {
                        char *cpy = malloc(4096);
                        if (is_ionumber(parse + j))
                        {
                            my_strncpy(cpy, parse, strlen(parse));
                            cpy[strlen(parse)] = '\0';
                        }
                        else
                        {
                            my_strncpy(cpy, parse, j);
                            cpy[j] = '\0';
                        }
                        add_token(&token, cpy);
                        free(cpy);
                    }
                    if (!is_ionumber(parse + j))
                    {
                        add_token(&token, grammar[i]);
                        parse = parse + j + strlen(grammar[i]);
                    }
                    else
                        parse += strlen(parse);
                    j = -1;
                    break;
                }
                else if (parse[0] == '\"' || parse[0] == '\'')
                {
                    char *quote = parse_quote(&parse, &pt);
                    add_token(&token, quote);
                    free(quote);
                    j = 0;
                    break;
                }
            }
            j++;
        }
        if (!parse)
            break;
        if (parse[0] != '\0')
            add_token(&token, parse);
        pt = pt + strlen(parse);
        parse = strtok(NULL, delim);
    }
    add_token(&token, "\n");
    free(str2);
    return token;
}

/**
 ** \brief Create the chain list from a file
 ** \param f the name of the file
 ** \param token the chain list of token.
 ** \return The new chain list of token.
 **/
struct Token *read_file(char *f, struct Token *token)
{
    if (!f)
    {
        char str[4095];
        while (fgets(str, 4095, stdin))
        {
            if (!token)
                token = create_token(token, str);
            else
            {
                struct Token *cpy = token;
                while (cpy->next)
                    cpy = cpy->next;
                cpy->next = create_token(cpy->next, str);
            }
        }
    }
    else
    {
        FILE *file = fopen(f, "r+");
        if (!file)
            return NULL;
        char str[4095];
        while (fgets(str, 4095, file))
        {
            if (!token)
                token = create_token(token, str);
            else
            {
                struct Token *cpy = token;
                while (cpy->next)
                    cpy = cpy->next;
                cpy->next = create_token(cpy->next, str);
            }
        }
    }
    return token;
}
