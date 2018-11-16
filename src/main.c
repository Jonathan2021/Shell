#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include "include/shell.h"
#include "lexer/include/lexer_struct.h"
#include "lexer/include/my_tree.h"
#include "lexer/include/rule.h"
#include "print_ast/include/print_ast.h"


void add_token(struct Token **token, char *str)
{
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

struct Token *parse_path(struct Token *token, char **argv, long argc)
{
    int c = 0;
    static struct option long_options[] =
        { {"version",no_argument,0,'v'},
          {"ver",no_argument,0,'v'},
          {"norc",  no_argument, 0,'n'},
          {"ast-print",  no_argument, 0, 'a'},
          {"type-print",  no_argument, 0, 't'},
          {0, 0, 0, 0},
        };
    int option_index = 0;
    optind = 0;
    while ((c = getopt_long (argc,argv, "c:vnat",
        long_options, &option_index)) != -1)
    {
        if (c == 'c')
        {
            char *parse;
            char *delim = {"\t \n"};
            parse = strtok(optarg,delim);
            while (parse)
            {
                add_token(&token,parse);
                parse = strtok(NULL,delim);
            }
        }
        else if (c == 'a')
            set_value("--ast-print", "1");
        else if (c == 'n')
            reset_value();
        else if (c == 'v')
            set_value("version", "1");
        else if (c == 't')
            set_value("--type-print", "1");
        else
            fprintf(stderr,"[GNU long options] [options] [file]\n");
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
        return t;
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


struct Token *carving(long argc, char **argv)
{
    char str[4095];
    printf("42sh$ ");
    struct Token *token = NULL;
    while(fgets(str,4095,stdin))
    {
        token = NULL;
        if (strncmp(str,"exit",4) == 0)
        {
            return 0;
        }
        argc = str_to_argv(argv,str);
        token = parse_path(token,argv,argc);
        lexer(token);
        DestroyToken(token);
        if (check_option(token))
        {
            reset_value();
            exit(0);
        }
        printf("42sh$ ");
    }
    return 0;
}
int main(int argc, char *argv[])
{
    FILE *file = fopen("src/file/variable.txt","w+");
    fprintf(file,"IFS \"\\t \\n\"\n--ast-print \"0\"\nversion \"0\"\n--type-print \"0\"\n");
    fclose(file);
    if (isatty(0))
        carving(argc,argv);
    else
        read_isatty();
    reset_value();
    return 0;
} 
