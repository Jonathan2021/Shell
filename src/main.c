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

void add_token(struct token **token, char *str)
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
    struct token *next = malloc(sizeof(struct token));
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
    struct token *copy = *token;
    while (copy->next)
    {
        copy = copy->next;
    }
    copy->next = next;
}

struct token *parse_path(struct token *token,char * str)
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

int main(void)
{
    //if (isatty(0))
    char str[4095];
    if (isatty(0))
        printf("42sh$ ");
    struct token *token = NULL;
    while(fgets(str,4095,stdin))
    {
        token = parse_path(token,str);
        while(token)
        {
            printf("->%s",token->type);
            token = token->next;
        }
        if (isatty(0))
            printf("\n42sh$ ");
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
