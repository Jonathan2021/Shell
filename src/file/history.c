#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../include/shell.h"

int line = 0;

void write_history(char *str)
{
    FILE *file = fopen(".42sh_history", "a+");
    char *parse;
    char *delim = {"\t "};
    parse = strtok(str,delim);
    int i = 0;
    while (parse)
    {
        if (i == 0)
            fprintf(file,"(%i)",line);
        fprintf(file," %s",parse);
        parse = strtok(NULL,delim);
        i++;
    }
    fclose(file);
    line ++;
}

void delete_history(void)
{
    remove(".42sh_history");
}