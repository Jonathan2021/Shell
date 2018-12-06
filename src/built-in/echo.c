#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char special[8][2] = {
    {'\\', '\\'},
    {'a', '\a'},
    {'b', '\b'},
    {'f', '\f'},
    {'n', '\n'},
    {'r', '\r'},
    {'t', '\t'},
    {'v', '\v'}
};

void change_backslash(char *txt, int i, char replace)
{
    int j = 0;
    for (; txt[j] != '\0' ; j++)
    {
        if (j == i)
        {
            txt[j] = replace;
        }
        else if (j > i)
        {
            txt[j] = txt[j+1];
        }
    }
    txt = realloc(txt, (j-1)*sizeof(char));
}


char *replace(char *txt, int i, int nb, char new)
{
    int j = 0;
    for (; txt[j+nb] != '\0' ; j++)
    {
        if (j == i)
        {
            txt[j] = new;
        }
        else if (j > i)
        {
            txt[j] = txt[j+nb];
        }
    }
    txt[j] = '\0';
    txt = realloc(txt, (j)*sizeof(char));
    return txt;
}

char *oct_find(char *txt, int i)
{
    char oct[3] = {0};
    int j = 0;
    while (j < 3)
    {
        if ('0' <= txt[i+2+j] && txt[i+2+j] <= '8')
            oct[j] = txt[i+2+j];
        else
            break;
        j++;
    }
    char new = strtol(oct, 0, 8);
    return replace(txt, i, j+1, new);
}

char* hex_find(char *txt, int i)
{
    char hex[2] = {0};
    int j = 0;
    while (j < 2)
    {
        if ('0' <= txt[i+2+j] && txt[i+2+j] <= '9')
            hex[j] = txt[i+2+j];
        else if ('a' <= txt[i+2+j] && txt[i+2+j] <= 'f')
            hex[j] = txt[i+2+j];
        else if ('A' <= txt[i+2+j] && txt[i+2+j] <= 'F')
            hex[j] = txt[i+2+j];
        else
            break;
        j++;
    }
    char new = strtol(hex, 0, 16);
    return replace(txt, i, j+1, new);
}

char *remove_next(char *txt, int i)
{
    int j = 0;
    for (; txt[j] != '\0' ; j++)
    {
        if (j >= i)
        {
            txt[j] = txt[j+1];
        }
    }
    txt = realloc(txt, (j-1)*sizeof(char));
    return txt;
}

int find_e(char *txt, int i)
{
    int j = i;
    while (txt[j] != '\0')
    {
        if (txt[j] == '\\')
        {
            for (int h = 0; h < 8; h++)
            {
                if (special[h][0] == txt[j+1])
                {
                    j = j + 2;
                    continue;
                }
            }
            if (txt[j+1]== 'e' || txt[j+1]== 'c')
            {
                j = j +2;
                continue;

            }
            else if (txt[j+1]== 'x')
            {
                hex_find(txt, j);
                return j;
            }
            else if (txt[j+1] == '0')
            {
                oct_find(txt, j);
                return j;
            }
            else
            {
                return j;
            }
        }
        else if (txt[j] == ' ')
        {
            j++;
            continue;
        }
        else
            return j;
    }
    return j;
}

char *change_txt(char *txt)
{
    for (int i = 0; txt[i] != '\0'; i++)
    {
        if (txt[i] == '\\')
        {
            for (int j = 0; j < 8; j++)
            {
                if (special[j][0] == txt[i+1])
                {
                    change_backslash(txt, i, special[j][1]);
                    break;
                }
            }
            if (txt[i+1]== 'e')
            {
                remove_next(txt, i);
                remove_next(txt, i);
                int pos = find_e(txt, i);
                remove_next(txt, pos);
            }
            if (txt[i+1]== 'c')
            {
                txt = realloc(txt, (i-1)*sizeof(char));
                txt[i] = '\0';
                return txt;
            }
            if (txt[i+1]== 'x')
                hex_find(txt, i);
            if (txt[i+1]== '0')
                oct_find(txt, i);
        }
    }
    return txt;
}

int option(char **args, int *flag)
{
    int i = 0;
    flag[0] = 0;
    flag[1] = 0;
    while (args[i] != NULL && args[i][0] == '-')
    {
        if ((strcmp(args[i], "-n")) == 0)
            flag[0] = 1;
        if ((strcmp(args[i], "-e")) == 0)
            flag[1] = 1;
        if ((strcmp(args[i], "-E")) == 0)
            flag[1] = 0;
        i++;
    }
    return i;
}

int my_echo(char **args)
{
    int flag[2] = {0, 0};
    int i = option(args, flag);

    while (args[i] != NULL)
    {
        if (flag[1] == 0)
        {
            if (args[i+1] != NULL)
                fprintf(stdout, "%s ", args[i]);
            else
                fprintf(stdout, "%s", args[i]);
        }
        else
        {
            char *txt = change_txt(args[i]);
            if (args[i+1] != NULL)
                fprintf(stdout,"%s ", txt);
            else
                fprintf(stdout,"%s", txt);
        }
        i++;
    }
    if (flag[0] == 0)
        fprintf(stdout, "\n");
    return 0;
}

/*
int main(int argc, char *argv[])
{
    char **list = malloc((argc+1)*sizeof(char*));
    list[argc] = NULL;
    int s = 0;
    int i = 1;
    int j = 0;
    while (i < argc)
    {
        s = 0;
        for (; argv[i][s] != '\0'; s++);
        list[j] = malloc(s);
        list[j] = memcpy(list[j], argv[i], s);
        list[j][s] = '\0';
        i++;
        j++;
    }
    my_echo(list);
    return 1;
}*/
