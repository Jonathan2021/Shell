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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "../include/shell.h"

static int line_history = 0;

/**
 ** \brief Come back to the last command.
 ** \bug Don't use this function some problems
 ** \return The value 0;
 **/

int check_rl(void)
{
    rl_initialize();
    while(rl_bind_keyseq("\\e[a",up_arrow) ||
        rl_bind_keyseq("\\e[b",down_arrow));
    line_history = 0;
    return 0;
}

int up_arrow(void)
{
    if (history_length-line_history == 0)
        return 0;
    HIST_ENTRY *historique = history_get(history_length-line_history);
    rl_replace_line(historique->line, 0);
    rl_end_of_line(0, 0);
    line_history ++;
    return 0;
}

int down_arrow(void)
{
    if (history_length-line_history == 0)
        return 0;
    HIST_ENTRY *historique = history_get(history_length-line_history);
    rl_replace_line(historique->line, 0);
    rl_end_of_line(0, 0);
    line_history --;
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

void history(void)
{
    FILE *file = fopen(".42sh_history","r");
    char ligne[4096];
    int i = 1;
    while (fgets(ligne,4096,file) != NULL)
    {
        printf("%d %s\n",i,ligne);
        i ++;
    }
    fclose(file);
}

/**
 ** \brief Delete the .42sh_history file.
 **/
void delete_history(void)
{
    remove(".42sh_history");
}
