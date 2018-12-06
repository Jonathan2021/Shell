#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief checks if string is a number
 ** \param str string to be checked
 ** \return return 1 if it is a number, 0 otherwise
 **/
int isnumber(char *str)
{
    int i = 0;
    for (; str[i] != 0; ++i)
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            break;
    }
    return (!str[i]);
}

/**
 ** \brief converts a string to an int if it is a number
 ** \param str string to be converted
 ** \return returns the corresponding converted int on sucess (>=0),
 *-1 on failure
 **/
int get_fd(char *str)
{
    int fd;
    if (isnumber(str))
        fd = atoi(str);
    else
    {
        fprintf(stderr, "IONUMBER isn't a number\n");
        return -1;
    }
    return fd;
}

/**
 ** \brief closes file descriptors if they are correct and different from
 *STRDIN_FILENO, STDOUT_FILENO or STDERR_FILENO
 ** \param fd struct containing context dependent
 *input, output, and err file descriptors
 **/
void close_redirection(struct fds *fd)
{
    if (fd->in > 2)
        close(fd->in);
    if (fd->out > 2)
        close(fd->out);
    if (fd->err > 2)
        close(fd->err);
}

/**
 ** \brief Replaces input, output or error with a new file descriptor
 ** \param node pointer to the corresponding redirection node from the AST
 ** \param fd pointer to struct fds to modify with new file descriptor
 ** \param file new file descriptor
 ** \param io 0 to replace input, 1 for output and 2 for err
 ** \return 1 if io was 0, 1 or 2; 0 otherwise
 **/
int replace_fd(struct AST *node, struct fds *fd, int file, int io)
{
    if (node->child[0])
        io = get_fd(getvalue(node->child[0]->self->name));
    if (!io)
    {
        if (fd->in > 2)
            close(fd->in);
        fd->in = file;
    }
    else if (io == 1)
    {
        if (fd->out > 2)
            close(fd->out);
        fd->out = file;
    }
    else if (io == 2)
    {
        if (fd->err > 2)
            close(fd->err);
        fd->err = file;
    }
    else
        return 0;
    return 1;
}

/**
 ** \brief evaluation of the > or >| redirection to a file;
 ** \param node pointer to the > or >| node of the AST
 ** \param fd pointer to a structure containing file descriptors to modify
 **/
void greater(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = open(getvalue(path), O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (file == -1)
        fprintf(stderr, "greater: open failed\n");
    else if (!replace_fd(node, fd, file, 1))
        close(file);
}

/**
 ** \brief evaluation of the >> redirection to a file;
 ** \param node pointer to the >> node of the AST
 ** \param fd pointer to a structure containing file descriptors to modify
 **/
void dgreat(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = open(getvalue(path), O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file == -1)
        fprintf(stderr, "dgreat: open failed\n");
    else if (!replace_fd(node, fd, file, 1))
        close(file);
}

/**
 ** \brief evaluation of the < redirection to a file;
 ** \param node pointer to the < node of the AST
 ** \param fd pointer to a structure containing file descriptors to modify
 **/
void less(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = open(getvalue(path), O_RDONLY);
    if (file == -1)
    {
        perror("");
        return;
    }
    replace_fd(node, fd, file, 0);
}
/**
 ** \brief evaluation of the <> redirection to a file;
 ** \param node pointer to the <> node of the AST
 ** \param fd pointer to a structure containing file descriptors to modify
 **/
void lessgreat(struct AST *node, struct fds *fd)
{
    char *path = node->child[1]->self->name;
    int file = -1;
    int io = 0;
    if (node->child[0])
        io = get_fd(getvalue(node->child[0]->self->name));
    if (io)
        file = open(getvalue(path), O_RDWR | O_TRUNC | O_CREAT, 0644);
    else
        file = open(getvalue(path), O_RDWR | O_CREAT, 0644);
    if (file == -1)
    {
        perror("");
    }
    else if (!replace_fd(node, fd, file, 0))
        close(file);
}

/**
 ** \brief close input, output or error in fd depending on io
 ** \param fd pointer to the struct containing input, output and error file
 *descriptors
 ** \param io 0 for input, 1 for output and 2 for error
 **/
void my_close(struct fds *fd, int io)
{
    if (!io)
    {
        if (fd->in > 2)
            close(fd->in);
    }
    else if (io == 1)
    {
        if (fd->out > 2)
            close(fd->out);
    }
    else if (io == 2)
    {
        if (fd->err > 2)
            close(fd->err);
    }
}

/**
 ** \brief evaluation of the >& or <& redirection to a file;
 ** \param node pointer to the >& or <& node of the AST
 ** \param fd pointer to a structure containing file descriptors to modify
 ** \param io the default file descriptor to change depending on the
 *redirection
 **/
void something_and(struct AST *node, struct fds *fd, int io)
{
    int file = -1;
    // int old_io = io;
    if (node->child[0])
        io = get_fd(getvalue(node->child[0]->self->name));
    if (isnumber(node->child[1]->self->name))
    {
        file = get_fd(node->child[1]->self->name);
        errno = 0;
        if (file < 0 || file > 2 || (!file && (io > 0)) || (file && !io))
        {
            fprintf(stderr, "%d isn't open for the input or output\n", file);
            return;
        }
        if (!io)
        {
            if (fd->in > 2)
                close(fd->in);
            fd->in = (file == 1) ? fd->out : fd->err;
        }
        else if (io == 1)
        {
            if (fd->out > 2)
                close(fd->out);
            fd->out = (!file) ? fd->in : fd->err;
        }
        else if (io == 2)
        {
            if (fd->err > 2)
                close(fd->err);
            fd->err = (!file) ? fd->in : fd->err;
        }
        else
        {
            fprintf(stderr, "IONUMBER is invalid, should be 0, 1 or 2\n");
        }
    }
    else if (!strcmp(node->child[1]->self->name, "-"))
    {
        my_close(fd, io);
    }
    // else behaviour is not specified
}

/**
 ** \brief Checks the redirection and calls the appropriate function
 ** \param node pointer to the redirection node in the AST
 ** \param fd pointer to a struct containing file descriptors modified later on
 **/
void redirect_word(struct AST *node, struct fds *fd)
{
    if (!strcmp(node->self->name, ">"))
        greater(node, fd);
    else if (!strcmp(node->self->name, "<"))
        less(node, fd);
    else if (!strcmp(node->self->name, ">>"))
        dgreat(node, fd);
    else if (!strcmp(node->self->name, ">&"))
        something_and(node, fd, 1);
    else if (!strcmp(node->self->name, "<&"))
        something_and(node, fd, 0);
    else if (!strcmp(node->self->name, ">|"))
        greater(node, fd);
    else if (!strcmp(node->self->name, "<>"))
        lessgreat(node, fd);
}

/**
 ** \brief checks sanity of the redirection node and calls the appropriate
 *function depending on if it redirect to a heredoc or not
 ** \param node pointer to the redirection node in the AST
 ** \param fd pointer to a struct containing file descriptors modifies later on
 **/
void my_redirection(struct AST *node, struct fds *fd)
{
    if (!node || node->nb_child < 2 || !node->child[1])
        return;
    if (!strcmp("WORD", node->child[1]->self->type))
        redirect_word(node, fd);
    // else if (!strcmp("HEREDOC", node->child[1]->self->type))
    //    redirect_heredoc(node, fd);
}

/**
 ** \brief Evaluates all the redirections until the next ;, \n or &
 ** \param node pointer to a node containing a bunch of redirections
 ** \param fds pointer to a struct containing file descriptors modified along
 *all the redirections
 ** \param index indicates from which child of node we should start evaluating
 **/
void get_redirection(struct AST *node, struct fds *fd, int index)
{
    if (!node)
        return;
    struct AST *cur_child;
    for (int i = index; i < node->nb_child && node->child[i]; ++i)
    {
        cur_child = node->child[i];
        if (!strcmp(cur_child->self->name, ";")
            || !strcmp(cur_child->self->name, "&")
            || !strcmp(cur_child->self->name, "\n"))
            break;

        if (!strcmp(cur_child->self->type, "REDIRECTION"))
            my_redirection(cur_child, fd);
    }
}

/**
 ** \brief load file descriptors from on structure to another
 ** \param fd pointer to the struct that will have its file descriptors
 *modified
 ** \param to_add struct containing file descriptors to load into the other
 **/
void merge_redirection(struct fds *fd, struct fds to_add)
{
    if (to_add.in != -1)
        fd->in = to_add.in;
    if (to_add.out != -1)
        fd->out = to_add.out;
    if (to_add.err != -1)
        fd->err = to_add.err;
}

/**
 ** \brief Initiates a redirection node
 ** \param token value of node->self, contains the name of the redirection
 ** \return The redirection node we just created or NULL on failure
 **/
struct AST *redirection_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    token->type = "REDIRECTION";
    node->self = token;
    return node;
}

/**
 ** \brief evaluation of the grammar rule for redirections
 ** \param t pointer to an element of the chained list of tokens
 ** \return a redirection node if it verified the grammar rule or NULL
 *otherwise
 **/
struct AST *redirection(struct Token **t)
{
    struct Token *pipe;
    struct Token *io = NULL;
    struct Token *type;
    char *list[9][2] = {
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
    if (strcmp("IO_NUMBER", tmp->type) == 0)
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
                if (io)
                    node->child[0] = word_init(io);
                node->child[1] = word_init(type);
                if (tmp->next)
                    tmp = tmp->next;
                *t = tmp;
                return node;
            }
        }
    }
    return NULL;
}
