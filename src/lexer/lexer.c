/**
 ** \file lexer/lexer.c
 ** \brief Dtermine each tokens and each options
 ** \date 29 novembre 2018
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
/**
 ** \brief Check if the str is a io_number
 ** \param str str to ba analyse
 ** \return An integer. 0 is not an ionumber and 1 if it is one
 **/
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
/**
 ** \brief Add a token to the end of the chain list
 ** \param token the full chain list of tokens
 ** \param str the string to be put at the end of the chain list
 **/
void add_token(struct Token **token, char *str)
{
    char *grammar[3][20] =
    {{"SEMICOLON",";","&","\0"},
        {"OPERATOR","&&","||",";;", "<<", ">>", "<&", ">&", "<>", "<<-", ">|", 
            "|", "\0"},
        {"NEW_LINE","\n","\0"}};
    struct Token *next = malloc(sizeof(struct Token));
    next->name = NULL;
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; grammar[i][j][0] != '\0'; j++ )
        {
            if (strcmp(grammar[i][j],str) == 0)
            {
                char *cpy = malloc(4096);
                strcpy(cpy,str);
                next->name = cpy;
                next->type = grammar[i][0];
                next->next = NULL;
            }
        }
    }
    if (is_ionumber(str))
    {
        char *str1 = malloc(4096);
        char *str2 = malloc(4096);
        sscanf(str,"%[^'>','<']%s",str1,str2);
        next->name = str1;
        next->type = "IO_NUMBER";
        next->next = NULL;
        add_token(&next,str2);
    }
    if (!next->name)
    {
        char *cpy = malloc(4096);
        strcpy(cpy,str);
        next->name = cpy;
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

/**
 ** \brief Parse all the user input for the options
 ** \param token the full chain list of tokens
 ** \param argv the list of argument given by the user
 ** \param argc the number of argument given by the user
 ** \return the correct chain list without the options given by the user and olso init the struct ps.
 **/
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
          {"name-print",  no_argument, 0, 6},
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
        else if (c == 6)
            set_value("--name-print", "1");
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

/**
 ** \brief This function build the AST tree and execute it.Moreover,  create a dot file output.gv if the option are given by the user.
 ** \param t the full chain list of tokens with their types
 ** \return the chain list of token
 **/
struct Token *lexer(struct Token *t)
{
    struct AST *ast = input(&t);
    if (ast == NULL)
        return t;
    struct fds fd = {.in = 0, .out = 1, .err = 2};
    ast->foo(ast, fd);
    char *print = get_value("--ast-print");
    if (print && t && strcmp(print,"1") == 0)
        create_dot(ast, "output.gv");
    AST_destroy(ast);
    return t;
}

/**
 ** \brief Free the chain list of token
 ** \param the chain list of token.
 **/
void DestroyToken(struct Token *t)
{
    if (t != NULL)
        DestroyToken(t->next);
    if (t && t->name)
    {
        printf("t->name = %s\n",t->name);
        free(t->name);
    }
    free(t);
}
/**
 ** \brief Initializie a new struct of PS
 ** \return the new struct PS inititalizied
 **/
struct PS *get_ps(void)
{
    struct PS *p = malloc(sizeof(struct PS));
    p->name = NULL;
    p->value = NULL;
    return p;
}
/**
 ** \brief Inittialize the begin of the PS struct
 **/
void init_ps(void)
{
    ps = malloc(sizeof(struct PS));
    ps->name = NULL;
    ps->value = NULL;
}
