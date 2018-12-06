/**
 ** \file parser/rule_compound_list.c
 ** \brief all about the compound_list say in the subject
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>
#include "../include/shell.h"
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/redirection.h"
#include "include/rule.h"
#include "../built-in/built-in.h"

/**
 ** \brief Execute the command pass in parameter and put in the good fd
 ** \param cmd list of command to execute
 ** \param fd the struture of file descrptor
 ** \return The return of the command executed
 **/



void expand_tilde(char *name)
{
    int j = 0;
    for (; name[j]; ++j)
    {
        if (name[j] == '/')
            break;
    }
    char old = name[j];
    name[j] = 0;
    if(!strcmp(name, "~"))
    {
        name[j] = old;
        size_t new_size = strlen(name) + strlen(getenv("HOME"));
        name = realloc(name, new_size);
        memmove(name + new_size - 1 - strlen(name + 1), name + 1, 
                strlen(name + 1));
        memmove(name, getenv("HOME"), strlen(getenv("HOME")));
        name[new_size - 1] = 0;

    }

    else if(!strcmp(name, "~+"))
    {
        name[j] = old;
        size_t new_size = strlen(name) + strlen(getenv("PWD"));
        name = realloc(name, new_size);
        memmove(name + new_size - 1 - strlen(name + 2), name + 2, 
                strlen(name + 2));
        memmove(name, getenv("PWD"), strlen(getenv("PWD")));
        name[new_size - 1] = 0;
    }
    else if(!strcmp(name, "~-"))
    {
        name[j] = old;
        size_t new_size = strlen(name) + strlen(getenv("OLDPWD"));
        name = realloc(name, new_size);
        memmove(name + new_size - 1 - strlen(name + 2), name + 2, 
                strlen(name + 2));
        memmove(name, getenv("OLDPWD"), strlen(getenv("OLDPWD")));
        name[new_size - 1] = 0;
    } 
}

void malloc_list(char *list[])
{
    size_t len;
    for (int i = 0; list[i]; ++i)
    {
        len = strlen(list[i]);
        char *tmp = malloc(len + 1);
        memmove(tmp, list[i], len);
        tmp[len] = 0;
        expand_tilde(tmp);
        list[i] = tmp;
    }
}

void free_list(char *list[])
{
    for (int i = 0; list[i]; ++i)
    {
        free(list[i]);
    }
}

int builtin(char *cmd[])
{
    if (!strcmp(cmd[0], "cd"))
        return my_cd(cmd + 1);
    return -1;
}

int my_exec(char *cmd[], struct fds fd)
{
    malloc_list(cmd);
    int res = -1;
    if ((res = builtin(cmd)) != -1)
        return res;
    res = 0;
    pid_t pid = fork();
    if (pid == -1)
        fprintf(stderr, "fork failed in compound\n");
    if (!pid)
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
        if (fd.err != 2)
        {
            dup2(fd.err, 2);
            close(fd.err);
        }
        if (execvp(cmd[0], cmd) < 0)
        {
            fprintf(stderr, "execvp failed\n");
        }
        exit(0);
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

/**
 ** \brief Execute the command present in the AST node
 ** \param node the node to be evaluate
 ** \param index of the child to execute
 ** \param fd structure of file descriptor
 ** \return The return of the command executed
 **/
int exec_init(struct AST *node, int *index, struct fds fd)
{
    char *my_cmd[512];
    int i = 0;
    char *cur_name;
    char *cur_type;
    int special = 0;
    int res;
    struct fds redir = {.in = -1, .out = -1, .err = -1};
    get_redirection(node, &redir, *index);
    merge_redirection(&fd, redir);
    for (; *index < node->nb_child && node->child[*index] && i < 511;
         (*index)++, ++i)
    {
        cur_name = node->child[*index]->self->name;
        cur_type = node->child[*index]->self->type;
        if (!strcmp(cur_name, ";") || !strcmp(cur_name, "&")
            || !strcmp(cur_name, "\n") || !strcmp(cur_type, "REDIRECTION"))
        {
            break;
        }
        if (strcmp(cur_type, "WORD"))
        {
            if (!node->child[*index]->foo)
                fprintf(stderr, "node : %s has no foo\n",
                    node->child[*index]->self->type);
            node->child[*index]->foo(node->child[*index], fd);
            res = node->child[*index]->res;
            special = 1;
        }
        else
        {
            my_cmd[i] = getvalue(cur_name);
        }
    }
    (*index)++;
    my_cmd[i] = NULL;
    if (!special)
        res = my_exec(my_cmd, fd);
    close_redirection(&redir);
    return res;
}
/**
 ** \brief Execute the compound node
 ** \param node the node to be evaluate
 ** \param fd the struture of file descrptor
 **/
void foo_compound(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0])
        return;
    int index = 0;
    int res = 0;
    while (index < node->nb_child
           && strcmp(node->child[index]->self->type, "REDIRECTION"))
        res = exec_init(node, &index, fd);
    node->res = res;
}

/**
 ** \brief Add the new node to the child of compound
 ** \param compound is the node where we put the new child at the end
 ** \param new the node to put in child
 **/
void add_compound(struct AST *compound, struct AST *new)
{
    compound->nb_child++;
    compound->child
        = realloc(compound->child, compound->nb_child * sizeof(struct AST));
    compound->child[compound->nb_child - 1] = new;
}
/**
 ** \brief Initialization of the compound node
 ** \return The compound node but not with his true child
 **/
struct AST *compound_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    token->type = "COMPOUND";
    token->name = "compound";
    struct AST *node = AST_init(0);
    if (!node)
    {
        free(token);
        return NULL;
    }
    node->self = token;
    node->foo = foo_compound;
    return node;
}
/**
 ** \brief Create the compound node with his child
 ** \param t the chain list of tokens
 ** \return The node case_clause with his good child
 **/
struct AST *compound_list(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *and_or_ast;
    struct AST *separator;
    struct AST *compound = NULL;
    struct Token *cpy;
    while (strcmp(tmp->name, "\n") == 0)
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
    while (1)
    {
        cpy = tmp;
        if (cpy
            && (!strcmp(cpy->name, ";") || !strcmp(cpy->name, "&")
                   || !strcmp(cpy->name, "\n")))
        {
            separator = word_init(cpy);
            cpy = cpy->next;
            while (cpy && !strcmp(cpy->name, "\n"))
                cpy = cpy->next;
            if (!cpy || !(and_or_ast = and_or(&cpy)))
            {
                AST_destroy(separator);
                break;
            }
            add_compound(compound, separator);
            add_compound(compound, and_or_ast);
            tmp = cpy;
            *t = tmp;
        }
        else
            break;
    }
    if (tmp
        && (!strcmp(tmp->name, "&") || !strcmp(tmp->name, ";")
               || !strcmp(tmp->name, "\n")))
    {
        add_compound(compound, word_init(tmp));
        tmp = tmp->next;
        while (tmp && !strcmp(tmp->name, "\n"))
        {
            tmp = tmp->next;
        }
        *t = tmp;
    }
    return compound;
}
