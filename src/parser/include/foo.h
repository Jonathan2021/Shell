#ifndef _FOO_H_
#define _FOO_H_

#include "my_tree.h"

void foo_compound(struct AST *node, struct fds fd);
void next_token(struct Token **tmp);
void call_ps2(struct Token **t, struct Token **t2);
#endif /* !_FOO_H_*/
