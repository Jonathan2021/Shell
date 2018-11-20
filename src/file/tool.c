#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../include/shell.h"

char *get_value(char *name, struct PS *ps)
{
    struct PS *tmp = ps;
    while(tmp->name)
    {
        if (strcmp(tmp->name,name) == 0)
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void set_value(char *name, char *value, struct PS **ps)
{
    struct PS *tmp = *ps;
    while(tmp->name)
    {
        if (strcmp(tmp->name,name) == 0)
        {
            tmp->value = value;
            return;
        }
        tmp = tmp->next;
    }
    tmp->value = value;
    tmp->name = name;
    tmp->next = init_ps();
}

long str_to_argv(char **argv, char *str)
{
    char *parse;
    char *delim = {"\n \t"};
    parse = strtok(str,delim);
    long i = 1;
    char * cpy = malloc(4095);
    while (parse)
    {
        if (strlen(parse) > 1 && ((parse[0] == '\"' && parse[strlen(parse)-1] == '\"' ) ||
            (parse[0] == '\'' && parse[strlen(parse)-1] == '\'')))
        {
            argv[i] = parse+1;
            argv[i][strlen(argv[i])-1] = 0;
            i++;
        }
        else if (parse[0] == '\"' || strcmp(parse,"\"") == 0 ||
            parse[0] == '\'' || strcmp(parse,"\'") == 0)
        {
            if (parse[0] == '\"' || parse[0] == '\'')
                argv[i] = parse+1;
            parse = strtok(NULL,delim);
            while (parse && (parse[strlen(parse)-1] != '\"' && strcmp(parse,"\"") != 0 &&
                parse[strlen(parse)-1] != '\'' && strcmp(parse,"\'") != 0))
            {
                strcpy(cpy,parse);
                strcat(argv[i]," ");
                strcat(argv[i],cpy);
                parse = strtok(NULL,delim);
            }
            if (parse && strlen(parse) > 1 && (parse[strlen(parse)-1] == '\"' || parse[strlen(parse)-1] == '\''))
            {
                parse[strlen(parse)-1] = 0;
                strcpy(cpy,parse);
                strcat(argv[i]," ");
                strcat(argv[i],cpy);
            }
            i++;
        }
        argv[i] = parse;
        parse = strtok(NULL,delim);
        i++;
    }
    free(cpy);
    argv[i] = NULL;
    return i;
}

void reset_value(struct PS *ps)
{
    struct PS *tmp = ps;
    while(tmp)
    {
        if (strcmp(tmp->name,"IFS") == 0)
        {
            tmp->value = "\\t \\n";
        }
        else
        {
            tmp->value = "0";
        }
        tmp = tmp->next;
    }
}


void read_isatty(void)
{
    char str[4095];
    int ret = 0;
    struct Token *token = NULL;
    struct PS *ps = NULL;
    while(fgets(str,4095,stdin))
    {
        token = NULL;
        if (strncmp(str,"exit",4) == 0)
        {
            exit(0);
        }
        char *parse;
        char *delim = {"\t \n"};
        parse = strtok(str,delim);
        while (parse)
        {
            add_token(&token,parse);
            parse = strtok(NULL,delim);
        }
        struct Token *tmp = token;
        while (tmp)
        {
            printf("->%s",tmp->type);
            tmp = tmp->next;
        }
    }
    if (ret == 1)
    {
        lexer(token);
        printf("\n");
    }
    check_option(token,ps);
    reset_value(ps);
}

struct Token *read_file(char *f, struct Token *token)
{
    if (!f)
    {
        char str[4095];
        while(fgets(str,4095,stdin))
        {
            char *parse;
            char *delim = {"\t \n"};
            parse = strtok(str,delim);
            while (parse)
            {
                char *tok = malloc(strlen(str)*2);
                strcpy(tok,parse);
                add_token(&token,tok);
                parse = strtok(NULL,delim);
            }
        }
    }
    else
    {
        FILE *file = fopen(f,"r+");
        if (!file)
            return NULL;
        char str[4095];
        while(fgets(str,4095,file))
        {
            char *parse;
            char *delim = {"\t \n"};
            parse = strtok(str,delim);
            while (parse)
            {
                char *tok = malloc(strlen(str)*2);
                strcpy(tok,parse);
                add_token(&token,tok);
                parse = strtok(NULL,delim);
            }
        }
    }
    return token;
}