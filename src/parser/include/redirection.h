#ifndef _REDIR_H_
#define _REDIR_H_

#include "my_tree.h"

void get_redirection(struct AST *node, struct fds *fd, int index);
void close_redirection(struct fds *fd);
void init_static(struct fds fd);

#endif /* !_REDIR_H_ */
