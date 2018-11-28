#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include <unistd.h>
#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void foo_pipe(struct AST *node, struct fds fd)
{
    if(!node || !node->child[0] || !node->child[1])
        return;
    int pipefd[2];
    if(pipe(pipefd) == -1)
    {
        fprintf(stderr, "pipe went wrong\n");
        return;
    }
    pid_t cpid = fork();
    if  (cpid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        perror("pipe fork went wrong\n");
        return;
    }
    else if (!cpid)
    {
        struct fds newfd = { .in = pipefd[0], .err = fd.err, .out = fd.out};
        close(pipefd[1]);
        node->child[1]->foo(node->child[1], newfd);
        close(pipefd[0]);
    }
    else
    {
        struct fds newfd = { .in = fd.in, .err = fd.err, .out = pipefd[1]};
        close(pipefd[0]);
        node->child[0]->foo(node->child[0], newfd);
        close(pipefd[1]);
    }
}

struct AST *pipe_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_pipe;
    return node;
}

struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct AST *tmp_ast;
    struct AST *rattach;
    //struct AST *first_cmd;

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
    //first_cmd = rattach;
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
        origin->child[0] = rattach;
    else
        origin = rattach;
    return origin;
}
/*
struct AST *r_pipeline(struct Token **t)
{
    struct AST *left_child;
    struct AST *right_child;
    struct Token *name;

    struct Token *tmp = *t;
    if ((left_child = command(&tmp)) == NULL)
    {
        return NULL;
    }
    *t = tmp;
    if (tmp == NULL)
        return left_child;

    if (strcmp("|", tmp->name) != 0)
        return left_child;
    if (tmp == NULL)
        return left_child;
    name = tmp;
    tmp = tmp->next;

    while (tmp && !strcmp("\n", tmp->name))
        tmp = tmp->next;
    if (tmp == NULL)
        return left_child;

    if ((right_child = r_pipeline(&tmp)) == NULL)
    {
        return left_child;
    }
    struct AST *node = pipe_init(name);
    node->child[0] = left_child;
    node->child[1] = right_child;
    *t = tmp;
    return node;
}
struct AST *pipeline(struct Token **t)
{
    struct AST *origin = NULL;
    struct Token *tmp = *t;
    if (tmp && !strcmp("!", tmp->name))
    {
        origin = bang_init(tmp);
        tmp = tmp->next;
        if ((origin->child[0] = r_pipeline(&tmp)) == NULL)
        {
            AST_destroy(origin);
            return NULL;
        }
        else
        {
            *t = tmp;
            return origin;
        }
    }
    if ((origin = r_pipeline(&tmp)) == NULL)
    {
        return NULL;
    }
    *t = tmp;
    return origin;
}
*/
