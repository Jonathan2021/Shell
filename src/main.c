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
#include<time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "include/shell.h"
#include "parser/include/lexer_struct.h"
#include "parser/include/my_tree.h"
#include "parser/include/rule.h"
#include "print_ast/include/print_ast.h"

struct Token *carving(long argc, char **argv)
{
    char str[4095] = {0};
    struct Token *token = NULL;
    init_ps();
    int i = 0;
    while(1)
    {
        token = NULL;
        clock_t begin=clock();
        if (i == 0)
        {
            i = 1;
            token = parse_path(token,argv,argc);
        }
        else
        {
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
        time_out(begin);
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

