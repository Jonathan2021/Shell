#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void greater(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = open(getvalue(path), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (file == -1)
        fprintf(stderr, "greater: open failed\n");
    int io = 1;
    if (node->child[0])
        io = atoi(getvalue(node->child[0]->self->name));
    if(fd->out == io)
    {
        fd->out = file;
        if (io != 1)
            close(io);
    }
    if(fd->err == io)
    {
        fd->err = file;
        if (io != 2)
            close(io);
    }
}

void dgreat(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = open(getvalue(path), O_WRONLY | O_APPEND | O_CREAT, 0666);
    if(file == -1)
        fprintf(stderr, "dgreat: open failed\n");
    int io = 1;
    if (node->child[0])
        io = atoi(getvalue(node->child[0]->self->name));
    if (fd->out == io)
    {
        fd->out = file;
        if (io != 1)
            close(io);
    }
    if(fd->err == io)
    {
        fd->err = file;
        if (io != 2)
            close(io);
    }
}

//void less(struct AST *node, struct fds *fd)
//{

//}

void redirect_word(struct AST *node, struct fds *fd)
{
    if (!strcmp(node->self->name, ">"))
        greater(node, fd);
    //else if (!strcmp(node->self->name, "<"))
    //    less(node, fd);
    else if (!strcmp(node->self->name, ">>"))
        dgreat(node, fd);
    //else if (!strcmp(node->self->name, ">&"))
    //    greatand(node, fd);
    //else if (!strcmp(node->self->name, "<&"))
    //    lessand(node, fd);
    //else if (!strcmp(node->self->name, ">|"))
    //    clobber(node, fd);
    //else if (!strcmp(node->self->name, "<>"))
    //    lessgreat(node, fd);
    
}

void my_redirection(struct AST *node, struct fds *fd)
{
    if (!node || node->nb_child < 2 || !node->child[1])
        return;
    if (!strcmp("WORD", node->child[1]->self->type))
        redirect_word(node, fd);
    //else if (!strcmp("HEREDOC", node->child[1]->self->type))
    //    redirect_heredoc(node, fd);
}

struct AST *redirection_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *redirection(struct Token **t)
{
    struct Token *pipe;
    struct Token *io = NULL;
    struct Token *type;
    char *list[9][2] =
        {
            {">", "WORD"},
            {"<", "WORD"},
            {">>", "WORD"},
            {"<<", "HEREDOC"},
            {"<<-", "HEREDOC"},
            {">&", "WORD"},
            {"<&", "WORD"},
            {">|", "WORD"},
            {"<>", "WORD"},
        };
    struct Token *tmp = *t;
    if (strcmp("IONUMBER", tmp->type) == 0)
    {
        io = tmp;
        tmp = tmp->next;
        if (tmp == NULL)
            return NULL;
    }
    struct Token *cpy = tmp;
    for (int i = 0; i < 9; i++)
    {
        tmp = cpy;
        if (strcmp(list[i][0], tmp->name) == 0)
        {
            pipe = tmp;
            tmp = tmp->next;
            if (tmp == NULL)
                return NULL;
            if (strcmp(list[i][1], tmp->type) == 0)
            {
                type = tmp;
                struct AST *node = redirection_init(pipe);
                if(io)
                    node->child[0] = word_init(io);
                node->child[1] = word_init(type);
                *t = tmp;
                return node;
            }
        }    
    }
    return NULL;
}
