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
#include "lexer/include/lexer_struct.h"
#include "lexer/include/my_tree.h"
#include "lexer/include/rule.h"
#include "print_ast/include/print_ast.h"


void add_token(struct Token **token, char *str)
{
    if (strcmp(str,"--ast-print") == 0)
        return;
    char *grammar[20][20] =
    {{"SEMICOLON",";","\0"},
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
    next->name = NULL;
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
    if (!next->name)
    {
        next->name = str;
        next->type = "WORD";
        next->next = NULL;
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

void print_t(struct Token *t)
{
    for (; t != NULL; t = t->next)
        printf("\n%s", t->name);
}
struct Token *lexer(struct Token *t)
{
    struct AST *ast = input(&t);
    if (ast == NULL)
        printf("null");
    else
        create_dot(ast, "output.gv");
    AST_destroy(ast);
    return t;
}
void DestroyToken(struct Token *t)
{
    if (t != NULL)
        DestroyToken(t->next);
    free(t);
}

void read_isatty(void)
{
    char str[4095];
    int ret = 0;
    struct Token *token = NULL;
    while(fgets(str,4095,stdin))
    {
        char *cpy = malloc(4095);
        strcpy(cpy,str);
        ret = 0;
        if (strncmp(str,"exit",4) == 0)
        {
            free(cpy);
            exit(0);
        }
        token = parse_path(token,str);
 /*       if (check_option(str))
            ret = 1;*/
        free(cpy);
    }
    struct Token *tmp = token;
    while (tmp)
    {
        printf("->%s",tmp->type);
        tmp = tmp->next;
    }
    lexer(token);
    if (ret == 1)
    {
        char chaine[100] = "";
        FILE *file = fopen("output.gv","r");
        if (!file)
            return;
        printf("\n");
        while (fgets(chaine,100,file) != NULL)
        {
            printf("%s", chaine);
        }
        fclose(file);
    }
}

struct Token *carving(void)
{
    char str[4095];
    int ret = 0;
    if (isatty(0))
        printf("42sh$ ");
    else
        {
            read_isatty();
            return 0;
        }
    struct Token *token = NULL;
    while(fgets(str,4095,stdin))
    {
        char *cpy = malloc(4095);
        strcpy(cpy,str);
        ret = 0;
        token = NULL;
        if (strncmp(str,"exit",4) == 0)
        {
            free(cpy);
            exit(0);
        }
        token = parse_path(token,str);
        struct Token *tmp = token;
        while (tmp)
        {
            ret = 1;
            printf("->%s",tmp->type);
            tmp = tmp->next;
        }
        DestroyToken(tmp);
        if (ret == 1)
        {
            lexer(token);
            ast_print(cpy);
            printf("\n");
        }
        free(cpy);
        printf("42sh$ ");
        DestroyToken(token);
    }
    return 0;
}
int main(void)
{
    carving();
    return 0;
} 
