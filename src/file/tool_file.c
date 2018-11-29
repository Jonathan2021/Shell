/**
 ** \file file/tool_file.c
 ** \brief Same thing of tool.c but in a file
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

/**
 ** \brief Give the name of a variable in the /tmp/42shrc file
 ** \param name The name of the variable
 ** \return The value associated to the variable given in parameters
 **/

char *get_file(char *name)
{
    FILE *file = fopen("/tmp/42shrc", "r+");
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

/**
 ** \brief Set the name of a variable in the /tmp/42shrc file with value given
 *in parameter
 ** \param name The name of the variable
 ** \param value the correspondant value of the variable
 **/
void set_file(char *name, char *value)
{
    FILE *file = fopen("/tmp/42shrc", "r");
    if (!file)
        return;
    FILE *copy = fopen("/tmp/copy.txt", "w+");
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
    remove("/tmp/42shrc");
    free(check);
    fclose(file);
    fclose(copy);
    rename("/tmp/copy.txt", "/tmp/42shrc");
}
/**
 ** \brief Set the file /tmp/42shrc to the initail variable
 **/
void reset_file(void)
{
    set_file("PS1", "");
    set_file("PS2", "");
}
