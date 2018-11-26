#include <stdlib.h>
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "../include/shell.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include "include/rule.h"

int my_exec(char *cmd[], struct fds fd)
{
    int res = 0;
    pid_t pid = fork();
    if (pid == -1)
        fprintf(stderr, "fork failed in compound\n");
    if(!pid)
    {
        if (fd.in)
        {
            dup2(fd.in, 0);
            close(fd.in);
        }
        if (fd.out != 1)
        {
            dup2(fd.out, 1);
            close(fd.out);
        }
        if(fd.err != 2)
        {
            dup2(fd.err, 2);
            close(fd.err);
        }
        if(execvp(cmd[0], cmd) < 0)
        {
            fprintf(stderr, "execvp failed\n");
        }
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        res = WIFEXITED(wstatus);
        if (res)
            res = !WEXITSTATUS(wstatus);
    }
    return res;
}

int exec_init(struct AST *node, int *index, struct fds fd)
{
    char *my_cmd[512];
    int i = 0;
    char *cur_name;
    char *cur_type;
    int special = 0;
    int res;
    for(; *index < node->nb_child && node->child[*index] && i < 511; (*index)++, ++i)
    {
        cur_name = node->child[*index]->self->name;
        cur_type = node->child[*index]->self->type;
        if (!strcmp(cur_name, ";") || !strcmp(cur_name, "&") 
            || !strcmp(cur_name, "\n"))
        {
            my_cmd[i] = cur_name;
            break;
        }
        if (strcmp(cur_type, "WORD"))
        {
            node->child[*index]->foo(node->child[*index], fd);
            res =  node->child[*index]->res;
            special = 1;
        }
        else
            my_cmd[i] = cur_name;
    }
    (*index)++;
    i++;
    my_cmd[i] = NULL;
    if (!special)
        res = my_exec(my_cmd, fd);
    return res;

}

void foo_compound(struct AST *node, struct fds fd)
{
    if(!node || !node->child[0])
        return;
    int index = 0;
    int res = 0;
    while(index < node->nb_child)
        res = exec_init(node, &index, fd);
    node->res = res;
}

void add_compound(struct AST *compound, struct AST *new)
{
    compound->nb_child++;
    compound->child = realloc(compound->child, \
    compound->nb_child * sizeof(struct AST));
    compound->child[compound->nb_child-1] = new;
}

struct AST *compound_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    token->type = "COMPOUND";
    token->name = "compound";
    struct AST *node = AST_init(0);
    if(!node)
    {
        free(token);
        return NULL;
    }
    node->self = token;
    node->foo = foo_compound;
    return node;
}

struct AST *compound_list(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *and_or_ast;
    struct AST *separator;
    struct AST *compound = NULL;
    struct Token *cpy;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    if (!(and_or_ast = and_or(&tmp)))
        return NULL;
    *t = tmp;
    compound = compound_init();
    add_compound(compound, and_or_ast);
    *t = tmp;
    while(1)
    {
        cpy = tmp;
        if (cpy && (!strcmp(cpy->name,";") ||
            !strcmp(cpy->name,"&") ||
            !strcmp(cpy->name,"\n")))
        {
            separator = word_init(cpy);
            cpy = cpy->next;
            while (cpy && !strcmp(cpy->name,"\n"))
                cpy = cpy->next;
            if (!cpy || !(and_or_ast = and_or(&cpy)))
                break;
            add_compound(compound, separator);
            add_compound(compound, and_or_ast);
            tmp = cpy;
            *t = tmp;
        }
        else
            break;
    }
    if (tmp && (!strcmp(tmp->name,"&") ||
        !strcmp(tmp->name,";") ||
        !strcmp(tmp->name,"\n")))
    {
        add_compound(compound, word_init(tmp));
        tmp = tmp->next;
        while(tmp && !strcmp(tmp->name,"\n"))
        {
            tmp = tmp->next;
        }
        *t = tmp;
    }
    return compound;
}
