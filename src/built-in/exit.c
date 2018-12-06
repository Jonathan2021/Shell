#include <stdlib.h>
#include "built-in.h"
#include "../include/shell.h"

int my_exit(struct AST *ast, struct Token *token)
{
    AST_destroy(ast);
    DestroyToken(token);
    reset_value();
    exit(0);
}
