#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "include/shell.h"

char *get_value(char *name)
{
    FILE *file = fopen("file/variable.txt","r+");
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
            strcat(cpy,value+1);
            free(value);
            return cpy;
        }
    }
    fclose(file);
    free(check);
    return NULL;
}

void set_value(char *name, char *value)
{
    FILE *file = fopen("file/variable.txt","r");
    if (!file)
        return;
    FILE *copy = fopen("file/copy.txt","w+");
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
    remove("file/variable.txt");
    free(check);
    fclose(file);
    fclose(copy);
    rename("file/copy.txt","file/variable.txt");

}

void add_value(char *name, char *value)
{
    FILE *file = fopen("file/variable.txt","r");
}

int main(void)
{
    printf("%s",get_value("name"));
    set_value("name","");
    //add_value("TEST","tg");
}
