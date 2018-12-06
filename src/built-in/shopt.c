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

void print_if_on(char *str, struct fds fd)
{
    char *print = get_value(str);
    if (print && strcmp(print, "1") == 0)
    {
        dprintf(fd.out,"%s",str);
        int i = 0;
        while (strlen(str) + i < 20)
        {
            dprintf(fd.out," ");
            i++;
        }
        dprintf(fd.out,":ON\n");
    }
}

/**
 ** \brief print all shopt ON
 **/

void print_on(struct fds fd)
{
    print_if_on("ast_print",fd);
    print_if_on("dotglob",fd);
    print_if_on("expand_aliases",fd);
    print_if_on("extglob",fd);
    print_if_on("nocaseglob",fd);
    print_if_on("nullglob",fd);
    print_if_on("sourcepath",fd);
    print_if_on("xpg_echo",fd);
}

/**
 ** \brief print if str shopt is OFF
 ** \param str shopt
 **/

void print_if_off(char *str, struct fds fd)
{
    char *print = get_value(str);
    if (!print || strcmp(print, "0") == 0)
    {
        dprintf(fd.out,"%s",str);
        int i = 0;
        while (strlen(str) + i < 20)
        {
            dprintf(fd.out," ");
            i++;
        }
        dprintf(fd.out,":OFF\n");
    }
}

/**
 ** \brief print all shopt OFF
 **/

void print_off(struct fds fd)
{
    print_if_off("ast_print",fd);
    print_if_off("dotglob",fd);
    print_if_off("expand_aliases",fd);
    print_if_off("extglob",fd);
    print_if_off("nocaseglob",fd);
    print_if_off("nullglob",fd);
    print_if_off("sourcepath",fd);
    print_if_off("xpg_echo",fd);
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

int my_shopt(char **argv, struct fds fd)
{
    int i = 0;
    if (argv[0] == NULL)
    {
        print_on(fd);
        print_off(fd);
    }
    while (argv[i] != NULL && argv[i][0] == '-')
    {
        if ((strcmp(argv[i], "-s")) == 0)
            print_on(fd);
        else if ((strcmp(argv[i], "-u")) == 0)
            print_off(fd);
        else if ((strcmp(argv[i], "-q")) == 0)
            return check_shopt(argv,&i);
        else
        {
            print_on(fd);
            print_off(fd);
        }

        i++;
    }
    return 0;
}
