#ifndef MY_TREE_H
#define MY_TREE_H

#include <stddef.h>
#include <string.h>
#include "lexer_struct.h"

struct AST;
struct fds;

typedef void (*foo)(struct AST *node, __attribute__((unused)) struct fds foo);

struct AST
{
    int nb_child;
    struct AST **child;
    struct Token *self;
    foo foo;
    int res;
};

struct fds
{
    int in;
    int err;
    int out;
};

struct AST *AST_init(int nb_child);
void AST_print_infix(const struct AST *tree);
void add_list(struct AST *l, struct AST *a);
void free_l(struct AST *ast);
void AST_destroy(struct AST *s);

struct AST *and_init(struct Token *token);
void foo_and(struct AST *node, struct fds fd);

struct AST *or_init(struct Token *token);
void foo_or(struct AST *node, struct fds fd);

// un else if est un if qui est mis dans le else_body
struct AST *if_init(struct Token *token);
void foo_if(struct AST *node, struct fds fd);

struct AST *bang_init(struct Token *token);
void set_status(int res);
int is_delim(char *str);
char *copy_str(char *str);
int eval_node(struct AST *node);
int is_quoted(char *str);
#endif /* ! MY_TREE_H */
