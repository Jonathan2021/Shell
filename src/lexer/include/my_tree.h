#ifndef MY_TREE_H
#define MY_TREE_H

#include <stddef.h>
#include <string.h>
#include "lexer_struct.h"

struct AST;

typedef void (*foo)(struct AST *node);

struct AST
{
    int nb_child;
    struct AST **child;
    struct Token *self;
    foo foo;
    int res;
};

struct AST *AST_init(int nb_child);
void AST_print_infix(const struct AST *tree);
void add_list(struct AST *l, struct AST *a);
void free_l(struct AST *ast);
void AST_destroy(struct AST *s);

struct AST *and_init(struct Token *token);
void foo_and(struct AST *node);

struct AST *or_init(struct Token *token);
void foo_or(struct AST *node);

// un else if est un if qui est mis dans le else_body
struct AST *if_init(struct Token *token);
void foo_if(struct AST *node);

struct AST *bang_init(struct Token *token);
#endif /* ! MY_TREE_H */
