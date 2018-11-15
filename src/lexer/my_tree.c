#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"

struct AST *AST_init(int nb_child)
{
    struct AST *new = malloc(sizeof(struct AST));
    if (new == NULL)
        return NULL;
    new->child = malloc(sizeof(struct AST) * nb_child);
    new->nb_child = nb_child;
    for (int i = 0; i < nb_child; i++)
    {
        new->child[i] = NULL;
    }
    new->res = 0;
    return new;
}
/*
void AST_print_prefix(const struct AST *tree)
{
    if (tree != NULL)
    {
        printf("%s ",tree->self->name);
        for (int i = 0; i < tree->nb_child; i++)
        {
            AST_print_infix(tree->child[i]);
        }
    }
}*/

void AST_destroy(struct AST *s)
{
    if (s == NULL)
        return;

    for (int i = 0; i < s->nb_child; i++)
    {
        AST_destroy(s->child[i]);
    }
    free(s->child);
    free(s);
}
