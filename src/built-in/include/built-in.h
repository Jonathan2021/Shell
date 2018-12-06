#ifndef H_BUILTIN
#define H_BUILTIN

#include "../..//parser/include/my_tree.h"
int my_cd(char **str);
int my_echo(char **args, struct fds fd);
char *remove_next(char *txt, int i);
int my_exit(struct AST *ast, struct Token *token);
int my_shopt(char **str, struct fds fd);
#endif /* ! H_BUILTIN */
