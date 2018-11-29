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


char *get_value(char *name)
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

char *getvalue(char *name)
{
    if (name[0] == '$')
    {
        char *cpy = malloc(strlen(name)*10);
        strcpy(cpy,name+1);
        char *value = get_value(cpy);
        free(cpy);
        if (value)
            return value;
    }
    return name;
}

void setvalue(char *name, char *value)
{
    set_value(name,value);
}

void set_value(char *name, char *value)
{
    struct PS *tmp = ps;
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
    tmp->next = get_ps();
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

void reset_value(void)
{
    struct PS *tmp = ps;
    while(ps->name)
    {
        tmp = ps;
        ps = ps->next;
        free(tmp);
    }
    free(ps);
}

struct Token *create_token(struct Token *token, char *str)
{
    char *parse;
    char *delim = {"\t \n"};
    parse = strtok(str,delim);
    char *grammar[20] = {"&&","||",";;", "<<", ">>", "<&", ">&", "<>", "<<-", ">|",";","&",">","<"};
    while (parse)
    {
        for (int j = 0; parse[j] != '\0';)
        {
            for (int i = 0; grammar[i]; i++)
            {
                if (strncmp(parse+j,grammar[i],strlen(grammar[i])) == 0)
                {
                    if (j != 0)
                    {
                       char cpy[40960];
                        strncpy(cpy,parse,j);
                        add_token(&token,cpy); 
                    }
                    add_token(&token,grammar[i]);
                    parse = parse+j+strlen(grammar[i]);
                    j = -1;
                    break;
                }
            }
            j++;
        }
        if (parse[0] != '\0')
            add_token(&token,parse);
        parse = strtok(NULL,delim);
    }
    add_token(&token,"\n");
    return token;
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
                add_token(&token,parse);
                parse = strtok(NULL,delim);
            }
            add_token(&token,"\n");
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
                add_token(&token,parse);
                parse = strtok(NULL,delim);
            }
            add_token(&token,"\n");
        }
    }
    add_token(&token,"\n");
    return token;
}