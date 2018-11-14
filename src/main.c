#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "include/shell.h"


void add_token(struct Token **token, char *str)
{
    char *grammar[20][20] =
    {{"WORD","ls","pwd","cd","\0"},
        {"SEMICOLON",";","\0"},
        {"OP_LOGIQUE","&&","||",";;","\0"},
        {"OP_IO","<<",">>","<&",">&","<>","<<-","\0"},
        {"CLOBBER",">|","\0"},
        {"IF","if","\0"},
        {"FI","fi","\0"},
        {"THEN","then","\0"},
        {"ELIF","elif","\0"},
        {"ELSE","else","\0"},
        {"LOOP","case","esac","while","until","for","\0"},
        {"BRACE","{","}","!","\0"},
        {"IN","in","\0"}};
    struct Token *next = malloc(sizeof(struct Token));
    for(int i = 0; i < 12; i++)
    {
        for (int j = 0; grammar[i][j][0] != '\0'; j++ )
        {
            if (strcmp(grammar[i][j],str) == 0)
            {
                next->name = str;
                next->type = grammar[i][0];
                next->next = NULL;
            }
        }
    }
    if (!*token)
    {
        *token = next;
        return;
    }
    struct Token *copy = *token;
    while (copy->next)
    {
        copy = copy->next;
    }
    copy->next = next;
}

struct Token *parse_path(struct Token *token,char * str)
{
    char *parse;
    char *delim = {"\n\t "};
    parse = strtok(str,delim);
    while (parse)
    {
        add_token(&token,parse);
        parse = strtok(NULL,delim);
    }
    return token;
}

struct Token*carving(void)
{
    char str[4095];
    int ret = 0;
    if (isatty(0))
        printf("42sh$ ");
    struct Token *token = NULL;
    while(fgets(str,4095,stdin))
    {
        ret = 0;
        if (strncmp(str,"exit",4) == 0)
            exit(0);
        token = parse_path(token,str);
        while(token)
        {
            ret = 1;
            printf("->%s",token->type);
            token = token->next;
        }
        if (ret == 1)
            printf("\n");
        if (isatty(0))
            printf("42sh$ ");
    }
    if (!isatty(0))
    {
        while(token)
        {
            printf("->%s",token->type);
            token = token->next;
        }
    }
    printf("\n");
    return 0;
}
void print_t(struct Token *t)
{
    for (;t != NULL ; t = t->next)
    {
        printf("%s ",t->name);
    }
}

int main(void)
{
    carving();
    return 0;
} 
