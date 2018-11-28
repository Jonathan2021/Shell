#define _GNU_SOURCE
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <glob.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/shell.h"

int line = 0;

void writehistory(char *str)
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

int up_arrow()
{
	HIST_ENTRY *historique=history_get(history_length);
	rl_replace_line(historique->line,0);
	rl_end_of_line(0,0);
	return 0;
}
void init_history(void)
{
    using_history();
	FILE *handle=fopen(".42sh_history","r");
	if (handle==NULL) 
		handle=fopen(".42sh_history","w+");
	fclose(handle);
	//read_history(".42sh_history");
	//stifle_history(500);
	//write_history(".42sh_history");
}

void delete_history(void)
{
    remove(".42sh_history");
}

int my_getch()
{
    struct     termios info;
    char     sequence[10];
    int        svg,pos,entier;

    tcgetattr(STDIN_FILENO,&info);
    svg=info.c_lflag;
    info.c_lflag = ~ECHO & ~ICANON; // ~ECHO=noecho ICANON=mode canonique (pas d'attente de entrée)
    tcsetattr(STDIN_FILENO, TCSANOW, &info);
    pos=0;
    entier=getchar();
    if (entier==27)
    {
        entier=getchar();
        sequence[pos]=(char)entier;
        ++pos;
        if (entier=='[')
        {
            do
            {
                entier=getchar();
                sequence[pos]=(char)entier;
                ++pos;
            } while (entier<=0x40);
            sequence[pos]='\0';
            if (strcmp(sequence,"[A")==0) entier=0403;             // fleche haute=27,[A 
            else if (strcmp(sequence,"[21~")==0) entier=0420;     // touche F10=27,[21~
            else entier=0;
        }
        else
        {
            ungetc(entier,stdin);
            entier=0;
        }
    }
    info.c_lflag = svg;
    tcsetattr(STDIN_FILENO, TCSANOW, &info);
    return entier;
}