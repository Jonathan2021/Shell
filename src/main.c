/**
 ** \file main.c
 ** \brief Program for the main
 *     * \version 0.5
 ** \date 30 novembre 2018
 **
 ** File with the main
 **
 **/
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
#include <sys/wait.h>
#include <errno.h>
#include <glob.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "include/shell.h"
#include "lexer/include/lexer_struct.h"
#include "lexer/include/my_tree.h"
#include "lexer/include/rule.h"
#include "print_ast/include/print_ast.h"

struct PS *ps;

void add_token(struct Token **token, char *str)
{
    char *grammar[21][20] =
    {{"SEMICOLON",";","\0"},
        {"OP_LOGIQUE","&&","||",";;","\0"},
        {"NEW_LINE","\n","\0"},
        {"OP_IO","<<",">>","<&",">&","<>","<<-","\0"},
        {"CLOBBER",">|","\0"},
        {"IF","if","\0"},
        {"FI","fi","\0"},
        {"THEN","then","\0"},
        {"ELIF","elif","\0"},
        {"ELSE","else","\0"},
        {"DO","do","\0"},
        {"DONE","done","\0"},
        {"LOOP","case","esac","while","until","for","\0"},
        {"BRACE","{","}","!","\0"},
        {"IN","in","\0"}};
    struct Token *next = malloc(sizeof(struct Token));
    next->name = NULL;
    for(int i = 0; i < 15; i++)
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
            token = create_token(token,optarg);
            set_value("--exit", "1");
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
    if (get_value("--exit") == NULL && (argv[argc-1] || !isatty(0)))
    {
        token = NULL;
        if (isatty(0))
            token = read_file(argv[argc-1],token);
        else
            token = read_file(NULL,token);
    }
    return token;
}
/**
 ** \fn void print_t(struct Token *t)
 ** \brief Printing AST function.
 **
 ** \param t Adress of the pointer structure to be print.
 ** \return No return.
 **/
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
    struct fds fd = {.in = 0, .out = 1, .err = 2};
    ast->foo(ast, fd);
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

struct PS *get_ps(void)
{
    struct PS *p = malloc(sizeof(struct PS));
    p->name = NULL;
    p->value = NULL;
    return p;
}

void init_ps(void)
{
    ps = malloc(sizeof(struct PS));
    ps->name = NULL;
    ps->value = NULL;
}

struct Token *carving(long argc, char **argv)
{
    char str[4095] = {0};
    struct Token *token = NULL;
    init_ps();
    int i = 0;
    while(1)
    {
        token = NULL;
        if (i == 0)
        {
            i = 1;
            token = parse_path(token,argv,argc);
        }
        else
        {
            //rl_bind_keyseq("\e[A",up_arrow);
            printf("42sh$ ");
            char *check = fgets(str,4095,stdin);
            if (!check)
                continue;
            if (strncmp(str,"exit",4) == 0)
                return 0;
            if (check && (check[0] != '\n' && check[0] != '\0'))
            {
                add_history(check);
				append_history(1,".42sh_history");
            }
            token = create_token(token,str);
        }
        lexer(token);
        if (check_option(token))
        {
            reset_value();
            exit(0);
        }
        DestroyToken(token);
        if (!isatty(0))
            exit(0);
    }
    reset_value();
    return 0;
}
int main(int argc, char *argv[])
{
    delete_history();
    init_history();
    carving(argc,argv);
    return 0;
} 