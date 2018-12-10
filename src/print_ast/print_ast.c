/**
 ** \file print_ast/print_ast.c
 ** \brief function for creating the DOT file from the AST
 ** \date 29 novembre 2018
 **/

#define _GNU_SOURCE
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/shell.h"
#include "../parser/include/lexer_struct.h"
#include "../parser/include/my_tree.h"

// command for create picture of graph : dot -Tps output.gv -o outfile.ps

/**
 ** \brief return color of the type node
 ** \param cur ast tree
 ** \return color of node
 **/

char *color(struct AST *cur)
{
    char *grammar[34][20] = {{"SEMICOLON", "[color=aquamarine]"},
        {"BANG", "[color=aquamarine]"},
        {"PARENTHESE", "[color=aquamarine]"},
        {"SHELL COMMAND", "[color=aquamarine]"},
        {"CURLY", "[color=aquamarine]"},
        {"OPERATOR", "[color=antiquewhite4]"},
        {"IO_NUMBER", "[color=antiquewhite4]"},
        {"ASSIGMENT_WORD", "[color=antiquewhite4]"},
        {"NEW_LINE", "[color=antiquewhite4]"}, {"OP_IO", "[color=darkgreen]"},
        {"CLOBBER", "[color=darkslateblue]"}, {"IF", "[color=khaki1]"},
        {"WHILE", "[color=darkslateblue]"}, {"UNTIL", "[color=khaki1]"},
        {"FI", "[color=saddlebrown]"}, {"THEN", "[color=orange]"},
        {"ELIF", "[color=gold]"}, {"ELSE", "[color=orangered]"},
        {"LOOP", "[color=magenta]"}, {"BRACE", "[color=paleturquoise4]"},
        {"WORD", "[color=blue2]"}, {"LIST", "[color=green]"},
        {"COMPOUND", "[color=green2]"}, {"COMMAND", "[color=green3]"},
        {"CASE_CLAUSE", "[color=green4]"}, {" ", "[color=orange4]"},
        {"FOR", "[color=orange3]"}, {"CASE ITEM", "[color=orange3]"},
        {"DONE", "[color=green2]"}, {"DO", "[color=green1]"},
        {"CASE", "[color=green1]"}, {"SIMPLE COMMAND", "[color=green1]"},
        {"REDIRECTION", "[color=green1]"}, {"IN", "[color=black]"}};
    char *color = malloc(60);
    for (int i = 0; i < 34; i++)
    {
        if (strcmp(grammar[i][0], cur->self->type) == 0)
        {
            strcpy(color, grammar[i][1]);
            return color;
        }
    }
    return NULL;
}

/**
 ** \brief print the DOT file corresponding to the ast
 ** \param cur ast tree
 ** \param file file descriptor
 ** \param j identifier of node
 ** \return number of node
 **/

int print_ast(struct AST *cur, FILE *file, int j)
{
    int cpy = j;
    char *type = color(cur);
    fprintf(file, "\"(%d)%s\"%s\n", j, cur->self->name, type);
    for (int i = 0; i < cur->nb_child; i++)
    {
        if (!cur->child[i])
            continue;
        char *type_child = color(cur->child[i]);
        fprintf(file, "\"(%d)%s\"%s\n", cpy + 1, cur->child[i]->self->name,
            type_child);
        fprintf(file, "\"(%d)%s\" -> \"(%d)%s\"\n", j, cur->self->name,
            cpy + 1, cur->child[i]->self->name);
        free(type_child);
        cpy = print_ast(cur->child[i], file, cpy + 1);
    }
    free(type);
    return cpy;
}

/**
 ** \brief print on output the DOT file
 **/

void ast_print(void)
{
    char chaine[100] = "";
    FILE *file = fopen("output.gv", "r");
    if (!file)
        return;
    printf("\n");
    while (fgets(chaine, 100, file) != NULL)
    {
        printf("%s", chaine);
    }
    printf("\n");
    fclose(file);
}

/**
 ** \brief check all option and execute option
 ** \param token linked list
 ** \return exit value
 **/

int check_option(struct Token *token)
{
    char *print = get_value("--type-print");
    if (print && strcmp(print, "1") == 0)
    {
        struct Token *tmp = token;
        while (tmp)
        {
            printf("\033[35m-> %s \033[0m", tmp->type);
            if (strcmp(tmp->type, "NEW_LINE") == 0)
                printf("\n");
            tmp = tmp->next;
            if (!tmp)
                printf("\n");
        }
    }
    print = get_value("--name-print");
    if (print && strcmp(print, "1") == 0)
    {
        struct Token *tmp = token;
        while (tmp && tmp->name)
        {
            printf("\033[33m-> %s \033[0m", tmp->name);
            if (strcmp(tmp->type, "NEW_LINE") == 0)
                printf("\n");
            tmp = tmp->next;
            if (!tmp)
                printf("\n");
        }
    }
    print = get_value("--ast-print");
    if (print && token && strcmp(print, "1") == 0)
        ast_print();
    print = get_value("version");
    if (print && strcmp(print, "1") == 0)
    {
        printf("Version 1\n");
        return 1;
    }
    print = get_value("--exit");
    if (print && strcmp(print, "1") == 0)
        return 1;
    print = get_value("exit");
    if (print && strcmp(print, "1") == 0)
        return 1;
    return 0;
}

/**
 ** \brief create dot file and init it
 ** \param cur ast tree
 ** \param filename name of file
 **/

void create_dot(struct AST *cur, const char *filename)
{
    FILE *file = fopen(filename, "w+");
    fprintf(file, "digraph G {\n");
    print_ast(cur, file, 0);
    fprintf(file, "}");
    fclose(file);
}
