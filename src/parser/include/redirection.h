#ifndef _REDIR_H_
#define _REDIR_H_

#include "my_tree.h"

void get_redirection(struct AST *node, struct fds *fd, struct fds old, int index);
void merge_redirection(struct fds *fd, struct fds to_add);
void close_redirection(struct fds *fd);

#endif /* !_REDIR_H_ */
