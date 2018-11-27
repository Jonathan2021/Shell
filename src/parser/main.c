#include <stdio.h>
#include <stdlib.h>
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"
#include "../print_ast/include/print_ast.h"
struct Token* init(char *name, char *type)
{
    struct Token *l = malloc(sizeof(struct Token));
    l->name = name;
    l->type = type;
    return l;
}

struct Token *exemple()
{
    struct Token *res = init("for", "FOR");
    res->next = init("w", "WORD");
    res->next->next = init("in", "IN");
    res->next->next->next = init("1", "WORD");
    res->next->next->next->next = init(";", "SEMICOLON");
    res->next->next->next->next->next = init("do", "DO");
    struct Token *a = res->next->next->next->next->next;
    a->next = init("ls", "WORD");
    a->next->next = init(";", "SEMICOLON");
    a->next->next->next = init("done", "DONE");
    return res;
}

void print_t(struct Token *t)
{
    for (;t != NULL ; t = t->next)
    {
        printf("%s ",t->name);
    }
}

void f_tt(struct Token *t)
{
    if (t->next != NULL)
    {
        f_tt(t->next);
    }
    free(t);
}

int main()
{
    struct Token *t = exemple();
    print_t(t);
    struct Token *c = t;
    struct AST *tree = input(&t);
    if (tree == NULL)
        printf("null");
    AST_destroy(tree);
    f_tt(c);
}
