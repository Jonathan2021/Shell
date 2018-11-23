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
#include "../include/shell.h"
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
        {"LIST","[color=green]"},
        {"COMPOUND","[color=green2]"},
        {"COMMAND","[color=green3]"},
        {"CASE_CLAUSE","[color=green4]"},
        {" ","[color=orange4]"},
        {"IN","[color=black]"}};
    char *color = malloc(60);
    for(int i = 0; i < 17; i++)
    {
        if (strcmp(grammar[i][0],cur->self->type) == 0)
        {
            strcpy(color,grammar[i][1]);
            return color;
        }
    }
    return NULL;
}

int print_ast(struct AST *cur, FILE *file, int j)
{
    int cpy = j;
    char *type = color(cur);
    fprintf(file,"\"(%d)%s\"%s\n",j,cur->self->name,type);
    for (int i = 0; i < cur->nb_child && cur->child[i]; i++)
    {
        char *type_child = color(cur->child[i]);
        fprintf(file,"\"(%d)%s\"%s\n",cpy+1,cur->child[i]->self->name,type_child);
        fprintf(file,"\"(%d)%s\" -> \"(%d)%s\"\n",j,cur->self->name,cpy+1,cur->child[i]->self->name);
        free(type_child);
        cpy = print_ast(cur->child[i],file, cpy+1);
    }
    free(type);
    return cpy;
}

void ast_print(void)
{
    char chaine[100] = "";
    FILE *file = fopen("output.gv","r");
    if (!file)
        return;
    printf("\n");
    while (fgets(chaine,100,file) != NULL)
    {
        printf("%s", chaine);
    }
    printf("\n");
    fclose(file);
}

int check_option(struct Token *token)
{
    char *print = get_value("--type-print");
    if (print && strcmp(print,"1") == 0)
    {
        struct Token *tmp = token;
        while (tmp)
        {
            printf("->%s",tmp->type);
            if (strcmp(tmp->type,"NEW_LINE") == 0)
                printf("\n");
            tmp = tmp->next;
            if (!tmp)
                printf("\n");
        }
    }
    print = get_value("--ast-print");
    if (print && token && strcmp(print,"1") == 0)
        ast_print();
    print = get_value("version");
    if (print && strcmp(print,"1") == 0)
    {
        printf("Version 0.5\n");
        return 1;
    }
    return 0;
}

 void create_dot(struct AST *cur, const char *filename)
 {
    FILE *file = fopen(filename,"w+");
    fprintf(file,"digraph G {\n");
    print_ast(cur,file,0);
    fprintf(file,"}");
    fclose(file);
 }
