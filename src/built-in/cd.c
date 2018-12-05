/**
 ** \file built-in/cd.c
 ** \brief Programm for the buildin cd
 ** \date 5 decembre 2018
 ** File with all the cd built in
 **/

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "built-in.h"
/**
 ** \brief The same as the man
 ** \param dest the string destination
 ** \param src the string source
 ** \param n the n char to be copied
 ** \return the destination pointer
 **/
static char *my_strncat(char *dest, const char *src, size_t n)
{
    size_t dest_len = strlen(dest);
    size_t i;

    for (i = 0 ; i < n && src[i] != '\0' ; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';

    return dest;
}

/**
 ** \brief The same as the man section
 ** \param the string to count the number of char
 ** \return the number of char of the string s
 **/
static size_t my_strlen(char *s)
{
    size_t len = 0;
    for (; s[len] != '\0'; len++);
    return len;
}

/**
 ** \brief The function witch replace the ~ by the good path of the variable $HOME
 ** \param the string to put replace
 ** \return the new string
 **/
static char *replace(char *str)
{

    if (str == NULL)
        return NULL;
    char *tmp = malloc(4096);
    int j = 0;
    int i = 1;
    for (; str[i] != '\0'; i++, j++)
        tmp[j] = str[i];
    tmp[j] = '\0';
    char *rslt = malloc(4096);
    strcpy(rslt, getenv("HOME"));
    my_strncat(rslt, tmp, my_strlen(tmp));
    free(tmp);
    return rslt;
}

/**
 ** \brief the function which put you in the $HOME directory if no args pass in argument
 ** \return if the operation pass good or not
 **/


static int settohome(void)
{
    const char *const home = getenv("HOME");
    if (home)
    {
        printf("chdir(%s) = %d\n", home, chdir(home));
        if(chdir(home))
            return -1;
        return 0;
    }
    return -1;
}

/**
 ** \brief The main function of the cd built in
 ** \param the list of the argument of cd
 ** \return if the operation pass good or not
 **/
int my_cd(char **cd_argv)
{
    if (cd_argv == NULL || cd_argv[0] == NULL \
            || cd_argv[0][0] == '\0')
    {
        return settohome();
    }
    else
    {
        char tmp[2048];
        if (strncmp(cd_argv[0], "-",1) == 0)
        {
            printf("change on %s = %d\n" \
                    , cd_argv[0], chdir(getenv("OLDPWD")));
            return 0;

        }
        else
        {
            char before[4096];
            strcpy(before, cd_argv[0]);
            if (cd_argv[0][0] == '~')
                cd_argv[0] = replace(cd_argv[0]);
            DIR *dir = opendir(cd_argv[0]);
            if (!dir)
            {
                fprintf(stdout, \
                        "42sh: cd: %s Not a directory\n", before);
                if (before[0] == '~')
                    free(cd_argv[0]);
                return -1;
            }
            setenv("OLDPWD", getcwd(tmp, 2048), !0);
            printf("chnge on %s = %d\n", cd_argv[0] \
                    , chdir(cd_argv[0]));

            setenv("PWD", getcwd(tmp, 2048), !0);
            if (before[0] == '~')
                free(cd_argv[0]);
            closedir(dir);
            return 0;
        }
        return -1;
    }
}

