#ifndef H_BUILTIN
#define H_BUILTIN
#include "../parser/include/my_tree.h"
int my_cd(char **str);
int my_echo(char **args, struct fds fd);
char *remove_next(char *txt, int i);
#endif /* ! H_BUILTIN */
