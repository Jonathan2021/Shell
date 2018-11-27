#ifndef _FOO_H_
#define _FOO_H_

#include "my_tree.h"

void foo_compound(struct AST *node, struct fds fd);
void next_token(struct Token **tmp);
int my_exec(char *cmd[], struct fds fd);
struct Token *call_ps2(struct Token **t);
#endif /* !_FOO_H_*/
