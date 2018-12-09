/**
 ** \file parser/pipe_node.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/my_tree.h"
#include "include/rule.h"
#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 ** \brief execute with pipe
 ** \param node ast tree for execution
 ** \param file descriptor
 **/

void foo_pipe(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0] || !node->child[1])
        return;
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        fprintf(stderr, "pipe went wrong\n");
        return;
    }
    pid_t cpid = fork();
    if (cpid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        perror("pipe fork went wrong\n");
        return;
    }
    else if (!cpid)
    {
        struct fds newfd = {.in = fd.in, .err = fd.err, .out = pipefd[1]};
        close(pipefd[0]);
        node->child[0]->foo(node->child[0], newfd);
        close(pipefd[1]);
        exit(0);
    }
    else
    {
        struct fds newfd = {.in = pipefd[0], .err = fd.err, .out = fd.out};
        close(pipefd[1]);
        node->child[1]->foo(node->child[1], newfd);
        node->res = node->child[1]->res;
        close(pipefd[0]);
    }
}

/**
 ** \brief init pipe node
 ** \param token linked list
 ** \return node pipe
 **/

struct AST *pipe_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_pipe;
    return node;
}

/**
 ** \brief check grammar pipeline and create pipe node
 ** \param token linked list
 ** \return node pipe
 **/

struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct AST *tmp_ast;
    struct AST *rattach;
    // struct AST *first_cmd;

    struct Token *tmp = *t;
    if (tmp && !strcmp("!", tmp->name))
    {
        origin = bang_init(tmp);
        tmp = tmp->next;
    }
    if (!tmp || !(rattach = command(&tmp)))
    {
        AST_destroy(origin);
        return NULL;
    }
    // first_cmd = rattach;
    *t = tmp;
    while (1)
    {
        if (strcmp("|", tmp->name))
            break;
        tmp_ast = rattach;
        rattach = pipe_init(tmp);
        rattach->child[0] = tmp_ast;
        tmp = tmp->next;
        while (tmp && !strcmp("\n", tmp->name))
            tmp = tmp->next;
        if (!tmp || !(tmp_ast = command(&tmp)))
        {
            tmp_ast = rattach->child[0];
            AST_destroy(rattach);
            rattach = tmp_ast;
            break;
        }
        rattach->child[1] = tmp_ast;
        *t = tmp;
    }
    if (origin)
    {
        origin->child[0] = rattach;
        origin->self->type = "BANG";
    }
    else
        origin = rattach;
    return origin;
}
