/**
 ** \file file/tool.c
 ** \brief Functions for cutting the user input
 ** \date 29 novembre 2018
 **/
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
    {
        char *cpy = malloc(strlen(name) * 10);
        strcpy(cpy, name + 1);
        char *value = get_value(cpy);
        free(cpy);
        if (value)
            return value;
    }
    return name;
}

/**
 ** \brief Set the value of the name pass in parameter
 ** \param name is the option
 ** \param value is the value which gone a be associated to the option
 **/
void setvalue(char *name, char *value)
{
    set_value(name, value);
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
 ** \brief cutting the -c option
 ** \param argv list of the -c argument
 ** \param str the full argument of -c
 ** \return Number of argument cut in -c argument.
 **/
long str_to_argv(char **argv, char *str)
{
    char *parse;
    char *delim = {"\n \t"};
    parse = strtok(str, delim);
    long i = 1;
    char *cpy = malloc(4095);
    while (parse)
    {
        if (strlen(parse) > 1
            && ((parse[0] == '\"' && parse[strlen(parse) - 1] == '\"')
                   || (parse[0] == '\'' && parse[strlen(parse) - 1] == '\'')))
        {
            argv[i] = parse + 1;
            argv[i][strlen(argv[i]) - 1] = 0;
            i++;
        }
        else if (parse[0] == '\"' || strcmp(parse, "\"") == 0
                 || parse[0] == '\'' || strcmp(parse, "\'") == 0)
        {
            if (parse[0] == '\"' || parse[0] == '\'')
                argv[i] = parse + 1;
            parse = strtok(NULL, delim);
            while (parse
                   && (parse[strlen(parse) - 1] != '\"'
                          && strcmp(parse, "\"") != 0
                          && parse[strlen(parse) - 1] != '\''
                          && strcmp(parse, "\'") != 0))
            {
                strcpy(cpy, parse);
                strcat(argv[i], " ");
                strcat(argv[i], cpy);
                parse = strtok(NULL, delim);
            }
            if (parse && strlen(parse) > 1
                && (parse[strlen(parse) - 1] == '\"'
                       || parse[strlen(parse) - 1] == '\''))
            {
                parse[strlen(parse) - 1] = 0;
                strcpy(cpy, parse);
                strcat(argv[i], " ");
                strcat(argv[i], cpy);
            }
            i++;
        }
        argv[i] = parse;
        parse = strtok(NULL, delim);
        i++;
    }
    free(cpy);
    argv[i] = NULL;
    return i;
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
    parse = strtok(str, delim);
    char *grammar[20] = {")", "(", "&&", "||", ";;", "<<", ">>", "<&", ">&",
        "<>", "<<-", ">|", ";", "&", ">", "<"};
    while (parse)
    {
        for (int j = 0; parse[j] != '\0';)
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
            }
            j++;
        }
        if (parse[0] != '\0')
            add_token(&token, parse);
        parse = strtok(NULL, delim);
    }
    add_token(&token, "\n");
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
            char *parse;
            char *delim = {"\t \n"};
            parse = strtok(str, delim);
            while (parse)
            {
                char *tok = malloc(strlen(parse) * 2);
                strcpy(tok, parse);
                add_token(&token, tok);
                parse = strtok(NULL, delim);
            }
            add_token(&token, "\n");
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
            char *parse;
            char *delim = {"\t \n"};
            parse = strtok(str, delim);
            while (parse)
            {
                char *tok = malloc(strlen(parse) * 2);
                strcpy(tok, parse);
                add_token(&token, tok);
                parse = strtok(NULL, delim);
            }
            add_token(&token, "\n");
        }
    }
    add_token(&token, "\n");
    return token;
}
