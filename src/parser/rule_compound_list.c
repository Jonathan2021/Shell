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
#include "../built-in/include/built-in.h"

/**
 ** \brief Expands tilde accordingly to the SCL
 ** \param name string to expand
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
    else if(!strcmp(name, "~+") || !strcmp(name, "~-"))
    {
        const char *var = "OLDPWD";
        if (!strcmp(name, "~+"))
            var = "PWD";
        name[j] = old;
        size_t new_size = strlen(name) + strlen(getenv(var)) - 1;
        name = realloc(name, new_size);
        memmove(name + new_size - 1 - strlen(name + 2), name + 2, 
                strlen(name + 2));
        memmove(name, getenv(var), strlen(getenv(var)));
        name[new_size - 1] = 0;
    }
    else
        name[j] = old;
}

/**
 ** \brief returns an allocated copy of a string
 ** \param str string to be copied
 ** \return The allocated and \0 terminated string
 **/
char *copy_str(char *str)
{
    char *res = calloc(strlen(str) + 1, sizeof(char));
    memmove(res, str, strlen(str));
    return res;
}

/**
 ** \brief copies all the elements of a char ** list 
 ** \param list list of elements to be copied
 **/
void malloc_list(char *list[])
{
    for (int i = 0; list[i]; ++i)
    {
        list[i] = copy_str(list[i]);
    }
}

/**
 ** \brief frees the content of a char ** list and itself
 ** \param list the list to be freed
 ** \param size the size of list
 **/
void free_list(char *list[], size_t size)
{
    for (size_t i = 0; i < size && list[i] ; ++i)
    {
        free(list[i]);
    }
    free(list);
}

/**
 ** \brief Calls the project's reimplementation of the builtin if it exists
 ** \param cmd char* array with the name of the binary/builtin and its arguments
 ** \param fd struct contain input, output and error file descriptors
 ** \return return 1 if successful execution, 0 if not and -1 if the
 *binary/builtin was not reimplemented
 **/
int builtin(char *cmd[], struct fds fd)
{
    if (!strcmp(cmd[0], "cd"))
        return my_cd(cmd + 1);
    if (!strcmp(cmd[0], "echo"))
        return my_echo(cmd + 1, fd);
    if (!strcmp(cmd[0], "exit") && !cmd[1])
    {
        set_value("exit", "1");
        return 1;
    }
    return -1;
}

/**
 ** \brief Executes the binary/builtin using execvp if not reimplemented
 *in the project
 ** \param cmd char* array with the name of the binary/builtin and its arguments
 ** \param fd struct contain input, output and error file descriptors
 ** \return return 1 if successful execution, 0 otherwise
 *otherwise returns NULL
 **/
int my_exec(char *cmd[], struct fds fd)
{
    int res = -1;
    if ((res = builtin(cmd, fd)) != -1)
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
            exit(127);
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
 ** \brief checks if the string is quoted and calls the adequate function
 ** \param cur_name string to be checked
 ** \return The modified function if the string was quoted, 
 *otherwise returns NULL
 **/
char *double_quotes(char *cur_name)
{
    size_t j = 0;
    size_t len = 4096;
    char *fill = calloc(len, sizeof(char));
    for (size_t i = 0; cur_name[i] && j < len - 1; ++i)
    {
        if(j + 1 >= len - 1)
        {
            len *= 2;
            fill = realloc(fill, len);
        }
        if (cur_name[i] == ' ')
            fill[j++] = ' ';
        else
        {
            size_t old_i = i;
            for (; cur_name[i + 1] && cur_name[i + 1] != ' '; ++i);
            char c = cur_name[i + 1];
            cur_name[i + 1] = 0;
            char *tmp = getvalue(cur_name + old_i);
            size_t tmp_len = strlen(tmp);
            for (size_t k = 0; k < tmp_len; ++k)
            {
                fill[j++] = tmp[k];
            }
            cur_name[i + 1] = c;
        }
    }
    fill = realloc(fill, j + 1);
    fill[j] = 0;
    return fill;
}

/**
 ** \brief checks if the string is quoted and calls the adequate function
 ** \param cur_name string to be checked
 ** \return The modified function if the string was quoted, 
 *otherwise returns NULL
 **/
char *quotes(char *cur_name)
{
    char f = cur_name[0];
    char l = cur_name[strlen(cur_name) - 1];
    if (!((f == 39 || f == '"') && l == f && strlen(cur_name) > 2))
        return NULL;
    size_t cur_len = strlen(cur_name) - 1; 
    cur_name[cur_len] = 0;
    cur_name[0] = 0;
    char *fill = NULL;
    if (f == 39)
    {
        fill = calloc(strlen(cur_name + 1) + 1, sizeof(char));
        if (!fill)
            return NULL;
        memmove(fill, cur_name + 1, strlen(cur_name + 1));
    }
    else
    {
        fill = double_quotes(cur_name + 1);
    }
    cur_name[0] = f;
    cur_name[cur_len] = f;
    return fill;
}



/**
 ** \brief Execute the command present in the AST node
 ** \param node the node to be evaluated
 ** \param index of the child to execute
 ** \param fd structur of file descriptor
 ** \return The return value of the executed command
 **/
int exec_init(struct AST *node, int *index, struct fds fd)
{
    char **my_cmd = calloc(512, sizeof(char *));
    int i = 0;
    char *cur_name;
    char *cur_type;
    int res = 0;
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
            break;
        if (strcmp(cur_type, "WORD"))
        {
            if (node->child[*index]->foo)
            {
                node->child[*index]->foo(node->child[*index], fd);
                res = node->child[*index]->res;
            }
        }
        else
        {
            char *quote = NULL;
            if ((quote = quotes(cur_name)))
                my_cmd[i] = quote;
            else
            {
                my_cmd[i] = copy_str(getvalue(cur_name));
                expand_tilde(my_cmd[i]);
            }
        }
    }
    (*index)++;
    if (my_cmd[0])
        res = my_exec(my_cmd, fd);
    free_list(my_cmd, 512);
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
