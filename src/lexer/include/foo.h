#ifndef _FOO_H_
#define _FOO_H_

#include "my_tree.h"

void foo_compound(struct AST *node, struct fds fd);
struct Token *get_Token(struct Token **tmp, struct Token **t);
void call_ps(struct Token **t, struct Token **t2);
#endif /* !_FOO_H_*/
