/**
 ** \file lexer.c
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
#include<time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/shell.h"
#include "../parser/include/lexer_struct.h"
#include "../parser/include/my_tree.h"
#include "../parser/include/rule.h"
#include "../print_ast/include/print_ast.h"
#include <fnmatch.h>

struct PS *ps;

int is_ionumber(char *str)
{
    int i = 0;
    for(; str[i]; ++i)
    {
        if(!(str[i] >= '0' && str[i] <= '9'))
            break;
    }
    return (i && (str[i] == '<' || str[i] == '>'));
}

void add_token(struct Token **token, char *str)
{
    char *grammar[3][20] =
    {{"SEMICOLON",";","&","\0"},
        {"OPERATOR","&&","||",";;", "<<", ">>", "<&", ">&", "<>", "<<-", ">|", 
            "\0"},
        {"NEW_LINE","\n","\0"}};
    struct Token *next = malloc(sizeof(struct Token));
    next->name = NULL;
    for(int i = 0; i < 3; i++)
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
    if (is_ionumber(str))
    {
        char str1[4096];
        char str2[4096];
        sscanf(str,"%s %s",str1,str2);
        next->name = str1;
        next->type = "IO_NUMBER";
        next->next = NULL;
        add_token(&next,str2);
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
        { {"version",no_argument,0,3},
          {"ver",no_argument,0,3},
          {"norc",  no_argument, 0,4},
          {"ast-print",  no_argument, 0, 1},
          {"type-print",  no_argument, 0, 2},
          {"timeout",  required_argument, 0, 5},
          {0, 0, 0, 0},
        };
    int option_index = 0;
    optind = 0;
    int i = 0;
    while ((c = getopt_long (argc,argv, "c:t:",
        long_options, &option_index)) != -1)
    {
        if (c == 'c')
        {
            token = create_token(token,optarg);
            set_value("--exit", "1");
        }
        else if (c == 1)
            set_value("--ast-print", "1");
        else if (c == 4)
            reset_file();
        else if (c == 3)
            set_value("version", "1");
        else if (c == 2)
            set_value("--type-print", "1");
        else if (optarg && (c == 5 || c == 't'))
            set_value("--timeout", optarg);
        else
            fprintf(stderr,"[GNU long options] [options] [file]\n");
        i++;
    }
    if (get_value("--exit") == NULL && (argv[argc-1] || !isatty(0)))
    {
        token = NULL;
        if (isatty(0) && i < argc-1)
        {
            token = read_file(argv[argc-1],token);
            set_value("--exit", "1");
        }
        else if (!isatty(0))
            token = read_file(NULL,token);
    }
    return token;
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

void time_out(clock_t begin)
{
    clock_t end=clock();
    double times = (end-begin)/CLOCKS_PER_SEC;
    char *check = get_value("--timeout");
    if (check)
    {
        double wait = atoi(get_value("--timeout"));
        if (times > wait)
        {
            printf("TOO LONG !!\n");
            printf ("execution = %f\n",times);
            printf ("time set = %f\n",wait);
        }
        else
        {
            printf ("TIMES OK\n");
            printf ("execution = %f\n",times);
            printf ("time set = %f\n",wait);
        }
    }
}
