/**
 ** \file built-in/shopt.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
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
#include "include/built-in.h"
#include "../parser/include/lexer_struct.h"
#include "../parser/include/my_tree.h"

/**
 ** \brief print if str shopt is ON
 ** \param str shopt
 **/

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

/**
 ** \brief print all shopt ON
 **/

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

/**
 ** \brief print if str shopt is OFF
 ** \param str shopt
 **/

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

/**
 ** \brief print all shopt OFF
 **/

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

/**
 ** \brief check option of shopt
 ** \param argv list of arg
 ** \param i index of list
 **/

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

/**
 ** \brief same behavior as shopt builtins
 ** \param argv list of arg
 **/

int my_shopt(char **argv)
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
