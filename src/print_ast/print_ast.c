#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../lexer/include/lexer_struct.h"
#include "../lexer/include/my_tree.h"
// dot -Tps ast.gv -o outfile.ps

char *color(struct AST *cur)
{
    char *grammar[20][20] =
    {{"SEMICOLON","[color=aquamarine]"},
        {"OP_LOGIQUE","[color=antiquewhite4]"},
        {"OP_IO","[color=darkgreen]"},
        {"CLOBBER","[color=darkslateblue]"},
        {"IF","[color=khaki1]"},
        {"FI","[color=saddlebrown]"},
        {"THEN","[color=orange]"},
        {"ELIF","[color=gold]"},
        {"ELSE","[color=orangered]"},
        {"LOOP","[color=magenta]"},
        {"BRACE","[color=paleturquoise4]"},
        {"WORD","[color=blue2]"},
        {"LIST","[color=blue4]"},
        {"IN","[color=black]"}};
    char *color = malloc(60);
    for(int i = 0; i < 13; i++)
    {
        if (strcmp(grammar[i][0],cur->self->type) == 0)
        {
            strcpy(color,grammar[i][1]);
            return color;
        }
    }
    return NULL;
}

void print_ast(struct AST *cur, FILE *file)
{
    for (int i = 0; i < cur->nb_child && cur->child[i]; i++)
    {
        char *type = color(cur);
        char *type_child = color(cur->child[i]);
        fprintf(file,"\"%s\"%s\n",cur->child[i]->self->name,type_child);
        fprintf(file,"\"%s\"%s\n",cur->self->name,type);
        fprintf(file,"\"%s\" -> \"%s\"\n",cur->self->name,cur->child[i]->self->name);
        free(type);
        free(type_child);
        print_ast(cur->child[i],file);
    }
}

 void create_dot(struct AST *cur, const char *filename)
 {
    FILE *file = fopen(filename,"w+");
    fprintf(file,"digraph G {\n");
    print_ast(cur,file);
    fprintf(file,"}");
 }

/*
int main(void)
{
    struct Token *tok_cur = malloc(sizeof(struct AST));
    tok_cur->name = "ls";
    tok_cur->type = "WORD";
    struct AST *cur = malloc(sizeof(struct AST));
    cur->self = tok_cur;
    cur->nb_child = 3;
    cur->child = malloc(3 * sizeof(struct AST));
    
    struct Token *tok_child1 = malloc(sizeof(struct AST));
    tok_child1->name = "if";
    tok_child1->type = "IF";
    struct AST *child1 = malloc(sizeof(struct AST));
    child1->self = tok_child1;
    child1->nb_child = 0;

    struct Token *tok_child2 = malloc(sizeof(struct AST));
    tok_child2->name = "case";
    tok_child2->type = "LOOP";
    struct AST *child2 = malloc(sizeof(struct AST));
    child2->self = tok_child2;
    child2->nb_child = 0;

    struct Token *tok_child3 = malloc(sizeof(struct AST));
    tok_child3->name = "{";
    tok_child3->type = "BRACE";
    struct AST *child3 = malloc(sizeof(struct AST));
    child3->self = tok_child3;
    child3->nb_child = 0;

    cur->child[0] = child1;
    cur->child[1] = child2;
    cur->child[2] = child3;

    // dot -Tps ast.gv -o outfile.ps
    create_dot(cur,"ast.gv");
}*/
