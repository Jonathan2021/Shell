/**
 ** \file built-in/ariexp.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 ** \brief same behavior as strtok
 ** \param  str to split
 ** \param  delim char to split str
 **/

char **split(char *str, char delim)
{
    char **rslt = malloc(sizeof(char *) * strlen(str));
    int i = 0;
    int gi = 0;
    for (; str[i] != '\0'; i++)
    {
        char *tmp = malloc(strlen(str));
        int j = 0;
        for (; str[i] != delim && str[i] != '\0'; j++, i++)
        {
            if (str[i] != ' ')
                tmp[j] = str[i];
            else
                j--;
        }
        tmp[j] = '\0';
        rslt[gi++] = tmp;
        if (str[i] == '\0')
            break;
    }
    rslt[gi] = '\0';
    return rslt;
}

/**
 ** \brief same behavior as expen
 ** \param  str to expen
 **/

int expen(char *str)
{
    char **cut = split(str, '+');
    if (cut[1] == '\0')
        printf("NOT GOOD\n");
    else
        printf("%s add %s", getvalue(cut[0]), cut[1]);
    for (int i = 0; cut[i] != '\0'; i++)
        free(cut[i]);
    free(cut);
    return 0;
}
int main(int argc, char *argv[])
{
    expen("i    + w");
    return 0;
}
