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

char *get_file(char *name)
{
    FILE *file = fopen("src/etc/42shrc","r+");
    if (!file)
        return NULL;
    char *check = malloc(60);
    char *value = malloc(60);
    int nb;
    while((nb = fscanf(file,"%s %[^\n\t]s",check,value)) >= 0)
    {
        if (strcmp(check,name) == 0)
        {
            fclose(file);
            free(check);
            value[strlen(value)-1] = '\0';
            char *cpy = malloc(60);
            strcpy(cpy,value+1);
            free(value);
            return cpy;
        }
    }
    fclose(file);
    free(value);
    free(check);
    return NULL;
}

void set_file(char *name, char *value)
{
    FILE *file = fopen("src/etc/42shrc","r");
    if (!file)
        return;
    FILE *copy = fopen("src/etc/copy.txt","w+");
    char *check = malloc(60);
    char *data = malloc(60);
    int nb;
    while((nb = fscanf(file,"%s %[^\n\t]s",check,data)) >= 0)
    {
        if (strcmp(check,name) == 0)
        {
            fprintf(copy,"%s \"%s\"\n",check,value);
        }
        else
        {
            fprintf(copy,"%s %s\n",check,data);
        }
    }
    remove("src/etc/42shrc");
    free(check);
    fclose(file);
    fclose(copy);
    rename("src/etc/copy.txt","src/etc/42shrc");

}

void reset_file(void)
{
    set_file("PS1","");
    set_file("PS2","");
}
