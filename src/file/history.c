/**
 ** \file file/history.c
 ** \brief File with functions about the history of 42sh.
 ** \date 29 novembre 2018
 **
 **/
#define _GNU_SOURCE
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include "../include/shell.h"

int line = 0;
/**
 ** \brief Write the str in the .42sh_history file.
 ** \param str command write in the history
 **/
void writehistory(char *str)
{
    FILE *file = fopen(".42sh_history", "a+");
    char *parse;
    char *delim = {"\t "};
    parse = strtok(str, delim);
    int i = 0;
    while (parse)
    {
        if (i == 0)
            fprintf(file, "(%i)", line);
        fprintf(file, " %s", parse);
        parse = strtok(NULL, delim);
        i++;
    }
    fclose(file);
    line++;
}
/**
 ** \brief Come back to the last command.
 ** \bug Don't use this function some problems
 ** \return The value 0;
 **/
int up_arrow()
{
    HIST_ENTRY *historique = history_get(history_length);
    rl_replace_line(historique->line, 0);
    rl_end_of_line(0, 0);
    return 0;
}

/**
 ** \brief Crete the file if it doesn't exist.
 **/
void init_history(void)
{
    using_history();
    FILE *handle = fopen(".42sh_history", "r");
    if (handle == NULL)
        handle = fopen(".42sh_history", "w+");
    fclose(handle);
}

/**
 ** \brief Delete the .42sh_history file.
 **/
void delete_history(void)
{
    remove(".42sh_history");
}
