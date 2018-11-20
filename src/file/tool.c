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

char *get_value(char *name)
{
    FILE *file = fopen("src/file/variable.txt", "r+");
    if (!file)
        return NULL;
    char *check = malloc(60);
    char *value = malloc(60);
    int nb;
    while ((nb = fscanf(file, "%s %[^\n\t]s", check, value)) >= 0)
    {
        if (strcmp(check, name) == 0)
        {
            fclose(file);
            free(check);
            value[strlen(value) - 1] = '\0';
            char *cpy = malloc(60);
            strcpy(cpy, value + 1);
            free(value);
            return cpy;
        }
    }
    fclose(file);
    free(value);
    free(check);
    return NULL;
}

void set_value(char *name, char *value)
{
    FILE *file = fopen("src/file/variable.txt", "r");
    if (!file)
        return;
    FILE *copy = fopen("src/file/copy.txt", "w+");
    char *check = malloc(60);
    char *data = malloc(60);
    int nb;
    while ((nb = fscanf(file, "%s %[^\n\t]s", check, data)) >= 0)
    {
        if (strcmp(check, name) == 0)
        {
            fprintf(copy, "%s \"%s\"\n", check, value);
        }
        else
        {
            fprintf(copy, "%s %s\n", check, data);
        }
    }
    remove("src/file/variable.txt");
    free(check);
    fclose(file);
    fclose(copy);
    rename("src/file/copy.txt", "src/file/variable.txt");
}

long str_to_argv(char **argv, char *str)
{
    char *parse;
    char *delim = {"\n \t"};
    parse = strtok(str, delim);
    long i = 1;
    char *cpy = malloc(4095);
    while (parse)
    {
        if (strlen(parse) > 1
            && ((parse[0] == '\"' && parse[strlen(parse) - 1] == '\"')
                   || (parse[0] == '\'' && parse[strlen(parse) - 1] == '\'')))
        {
            argv[i] = parse + 1;
            argv[i][strlen(argv[i]) - 1] = 0;
            i++;
        }
        else if (parse[0] == '\"' || strcmp(parse, "\"") == 0
                 || parse[0] == '\'' || strcmp(parse, "\'") == 0)
        {
            if (parse[0] == '\"' || parse[0] == '\'')
                argv[i] = parse + 1;
            parse = strtok(NULL, delim);
            while (parse
                   && (parse[strlen(parse) - 1] != '\"'
                          && strcmp(parse, "\"") != 0
                          && parse[strlen(parse) - 1] != '\''
                          && strcmp(parse, "\'") != 0))
            {
                strcpy(cpy, parse);
                strcat(argv[i], " ");
                strcat(argv[i], cpy);
                parse = strtok(NULL, delim);
            }
            if (parse && strlen(parse) > 1
                && (parse[strlen(parse) - 1] == '\"'
                       || parse[strlen(parse) - 1] == '\''))
            {
                parse[strlen(parse) - 1] = 0;
                strcpy(cpy, parse);
                strcat(argv[i], " ");
                strcat(argv[i], cpy);
            }
            i++;
        }
        argv[i] = parse;
        parse = strtok(NULL, delim);
        i++;
    }
    free(cpy);
    argv[i] = NULL;
    return i;
}

void reset_value(void)
{
    set_value("IFS", "\\t \\n");
    set_value("--ast-print", "0");
    set_value("--type-print", "0");
    set_value("version", "0");
}

void read_isatty(void)
{
    char str[4095];
    int ret = 0;
    struct Token *token = NULL;
    while (fgets(str, 4095, stdin))
    {
        token = NULL;
        if (strncmp(str, "exit", 4) == 0)
        {
            exit(0);
        }
        // argc = str_to_argv(argv,str);
        // token = parse_path(token,argv,argc);
        char *parse;
        char *delim = {"\t \n"};
        parse = strtok(str, delim);
        while (parse)
        {
            add_token(&token, parse);
            parse = strtok(NULL, delim);
        }
        struct Token *tmp = token;
        while (tmp)
        {
            printf("->%s", tmp->type);
            tmp = tmp->next;
        }
    }
    if (ret == 1)
    {
        lexer(token);
        printf("\n");
    }
    check_option(token);
}
