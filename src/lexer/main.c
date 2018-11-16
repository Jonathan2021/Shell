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
    struct Token *res = init("if", "IF");
    res->next = init("pwd", "WORD");
    res->next->next = init("&&", "OP_LOGIQUE");
    res->next->next->next = init("ls", "WORD");
    res->next->next->next->next = init(";", "SEMICOLON");
    res->next->next->next->next->next = init("then", "THEN");
    struct Token *a = res->next->next->next->next->next;
    a->next = init("cd", "WORD");
    a->next->next = init(";", "SEMICOLON");
    a->next->next->next = init("else", "ELSE");
    a->next->next->next->next = init("alias", "WORD");
    a->next->next->next->next->next = init(";", "SEMICOLON");
    a->next->next->next->next->next->next = init("fi", "FI");
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
