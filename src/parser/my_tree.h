#ifndef MY_TREE_H
#define MY_TREE_H

#include <stddef.h>

struct AST
{
    int nb_child;
    struct AST** child;
    struct Token *self;
    int res;
};
struct AST *AST_init(int nb_child);
void AST_print_infix(const struct AST *tree);
void AST_destroy(struct AST *s);

struct AST *and_init();
int foo_and(struct AST *node);

struct AST *or_init();
int foo_or(struct AST *node);

//un else if est un if qui est mis dans le else_body
struct AST *if_init();
int foo_if(struct AST *node);
#endif /* ! MY_TREE_H */
