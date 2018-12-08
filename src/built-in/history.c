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

static int my_strlen(int x)
{
    int i = 1;
    while (x != 0)
    {
        x /= 10;
        i++;
    }
    return i;
}

void print_history(struct fds fd)
{
    FILE *file = fopen(".42sh_history", "r+");
    if (!file)
        return;
    char str[4095];
    int nb = 1;
    while (fgets(str, 4095, file))
    {
        int i = 0;
        while (my_strlen(nb) + i < 6)
        {
            dprintf(fd.out, " ");
            i++;
        }
        dprintf(fd.out, "%i  %s", nb, str);
        nb++;
    }
    fclose(file);
}

int my_history(char **argv, struct fds fd)
{
    int i = 0;
    if (argv[0] == NULL)
    {
        print_history(fd);
    }
    while (argv[i] != NULL && argv[i][0] == '-')
    {
        if ((strcmp(argv[i], "-c")) == 0)
        {
            delete_history();
            init_history();
            line_history = 0;
        }
        i++;
    }
    return 0;
}
