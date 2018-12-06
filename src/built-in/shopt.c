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

void print_if_on(char *str)
{
    char *print = get_value(str);
    if (print && strcmp(print, "1") == 0)
    {
        printf("%s",str);
        int i = 0;
        while (strlen(str) + i < 20)
        {
            printf(" ");
            i++;
        }
        printf(":ON\n");
    }
}

void print_on(void)
{
    print_if_on("ast_print");
    print_if_on("dotglob");
    print_if_on("expand_aliases");
    print_if_on("extglob");
    print_if_on("nocaseglob");
    print_if_on("nullglob");
    print_if_on("sourcepath");
    print_if_on("xpg_echo");
}

void print_if_off(char *str)
{
    char *print = get_value(str);
    if (!print || strcmp(print, "0") == 0)
    {
        printf("%s",str);
        int i = 0;
        while (strlen(str) + i < 20)
        {
            printf(" ");
            i++;
        }
        printf(":OFF\n");
    }
}

void print_off(void)
{
    print_if_off("ast_print");
    print_if_off("dotglob");
    print_if_off("expand_aliases");
    print_if_off("extglob");
    print_if_off("nocaseglob");
    print_if_off("nullglob");
    print_if_off("sourcepath");
    print_if_off("xpg_echo");
}

int check_shopt(char **argv, int *i)
{
    char *print;
    i[0] ++;
    while (argv[*i] != NULL && argv[*i][0] != '-')
    {
        print = get_value(argv[*i]);
        if (!print || strcmp(print, "0") == 0)
            return 0;
        i[0]++;
    }
    return 1;
}

int shopt(char **argv)
{
    int i = 0;
    if (argv[0] == NULL)
    {
        print_on();
        print_off();
    }
    while (argv[i] != NULL && argv[i][0] == '-')
    {
        if ((strcmp(argv[i], "-s")) == 0)
            print_on();
        else if ((strcmp(argv[i], "-u")) == 0)
            print_off();
        else if ((strcmp(argv[i], "-q")) == 0)
            return check_shopt(argv,&i);
        else
        {
            print_on();
            print_off();
        }

        i++;
    }
    return 0;
}