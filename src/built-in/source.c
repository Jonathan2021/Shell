#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "include/built-in.h"


int source(char **arg)
{
    if (arg == NULL || arg[0] == NULL)
    {
        fprintf(stderr,
            "42sh: source: filename argument required\nsource: usage: source filename [arguments]\n");
        return 2;
    }
    if (arg[1] != NULL)
        arg[1] = NULL;
    DIR* dir = opendir(arg[0]);
    if (dir)
    {
        fprintf(stderr ,"42sh: source: %s: is a directory\n", arg[0]);
        closedir(dir);
            return 1;
    }
    closedir(dir);
    if (access( arg[0], F_OK ) != -1)
    {
        pid_t child;
        char **argv = malloc(2 * sizeof(char *));
        argv[0] = malloc(84);
        argv[0][0] = '.';
        argv[0][1] = '/';
        argv[0][2] = '\0';
        argv[0] = strcat(argv[0], arg[0]);
        argv[1] = NULL;
        int child_status;
        child = fork();
        if (child == 0)
        {
            execvp(argv[0], argv);
            fprintf(stderr, "Cannot exec the bash script.\n");
            free(argv[0]);
            free(argv);
            exit(2);
        }
        if (child > 0)
        {
            free(argv[0]);
            free(argv);
            wait(&child_status);
            return child_status;
        }
    }
    else
    {
        fprintf(stderr, "42sh: %s: No such file\n", arg[0]);
        return 1;
    }
    return 0;

}
