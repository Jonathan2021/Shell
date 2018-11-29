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

/**
 ** \fn struct Token *carving(long argc, char **argv)
 ** \brief Carving the user input.
 **
 ** \param argc number of argument
 ** \param argv the list of argument
 ** \return The chain list of type strcut Token *.
 **/
struct Token *carving(long argc, char **argv)
{
    char str[4095] = {0};
    struct Token *token = NULL;
    init_ps();
    int i = 0;
    while (1)
    {
        token = NULL;
        if (i == 0)
        {
            i = 1;
            token = parse_path(token, argv, argc);
        }
        else
        {
            printf("%s", get_file("PS1"));
            char *check = fgets(str, 4095, stdin);
            if (!check)
                continue;
            if (strncmp(str, "exit", 4) == 0)
                return 0;
            if (check && (check[0] != '\n' && check[0] != '\0'))
            {
                add_history(check);
                append_history(1, ".42sh_history");
            }
            token = create_token(token, str);
        }
        lexer(token);
        if (check_option(token))
        {
            DestroyToken(token);
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
    fprintf(file, "PS1 \"42sh$ \"\nPS2 \">\"");
    fclose(file);
    delete_history();
    init_history();
    carving(argc, argv);
    return 0;
}
