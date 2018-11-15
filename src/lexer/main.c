#include <stdio.h>
#include <stdlib.h>
#include "include/lexer_struct.h"

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
    res->next->next = init("&&", "LOGICAL_AND");
    res->next->next->next = init("ls", "WORD");
    res->next->next->next->next = init(";", "SEMICOLON");
    res->next->next->next->next->next = init("then", "THEN");
    struct Token *a = res->next->next->next->next->next;
    a->next = init("cd", "WORD");
    a->next->next = init(";", "SEMICOLON");
    a->next->next->next = init("fi", "FI");
    //a->next->next->next->next = init("alias", "WORD");
    //a->next->next->next->next->next = init(";", "SEMICOLON");
    //a->next->next->next->next->next->next = init("fi", "FI");
    return res;
}

void print_t(struct Token *t)
{
    for (;t != NULL ; t = t->next)
    {
        printf("%s ",t->name);
    }
}
int main()
{
    struct Token *t = exemple();
    print_t(t);
    printf("%d", input(&t));
}
