/**
 ** \file parser/my_tree.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdlib.h>
#include <unistd.h>
#include "include/built-in.h"
#include "../include/shell.h"

/**
 ** \brief same behavior as exit() builtins
 ** \param  ast to free
 ** \param  token to free
 **/

int my_exit(struct AST *ast, struct Token *token)
{
    int res = atoi(getvalue("$?"));
    AST_destroy(ast);
    DestroyToken(token);
    reset_value();
    close(0);
    close(1);
    close(2);
    exit(res);
}

int exit_value(void)
{
    char *print = get_value("exit");
    if (print && strcmp(print, "1") == 0)
    {
        return 1;
    }
    return 0;
}
