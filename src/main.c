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
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "include/shell.h"
#include "parser/include/lexer_struct.h"
#include "parser/include/my_tree.h"
#include "parser/include/rule.h"
#include "print_ast/include/print_ast.h"

void sig(void)
{
    struct sigaction sac = {0};
    sac.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sac, NULL);
}

/**
 ** \fn struct Token *carving(long argc, char **argv)
 ** \brief Carving the user input.
 **
 ** \param argc number of argument
 ** \param argv the list of argument
 ** \return The chain list of type struct Token *.
 **/
struct Token *carving(long argc, char **argv)
{
    struct Token *token = NULL;
    init_ps();
    int i = 0;
    while (1)
    {
        check_rl();
        token = NULL;
        if (i == 0)
        {
            i = 1;
            token = parse_path(token, argv, argc);
        }
        else
        {
            sig();
            char *str = readline(getvalue("$PS1"));
            if (!str)
                continue;
            str[strlen(str)] = '\0';
            if (strncmp(str, "exit", 4) == 0)
            {
                free(str);
                return 0;
            }
            if (str && (str[0] != '\n' && str[0] != '\0'))
            {
                add_history(str);
                append_history(1, ".42sh_history");
            }
            token = create_token(token, str);
            free(str);
        }
        lexer(token);
        if (check_option(token))
        {
            DestroyToken(token);
            reset_value();
            exit(atoi(getvalue("$?")));
        }
        DestroyToken(token);
        if (!isatty(0))
            exit(atoi(getvalue("$?")));
    }
    reset_value();
    return 0;
}
/**
 ** \brief Main function in 42sh.
 **
 ** \param argc number of argument
 ** \param argv the list of argument
 ** \return The value 0
 **/
int main(int argc, char *argv[])
{
    FILE *file = fopen("/tmp/42shrc", "w+");
    fprintf(file, "PS1 \"42sh$ \"\nPS2 \"> \"");
    fclose(file);
    setvalue("PS1", "42sh$ ");
    setvalue("PS2", "> ");
    setvalue("echo-control-characters", "0");
    delete_history();
    init_history();
    set_status(0);
    carving(argc, argv);
    int res = atoi(getvalue("$?"));
    return res;
}
