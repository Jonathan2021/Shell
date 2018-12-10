/**
 ** \file parser/prefix.c
 ** \brief tools for ast
 ** \date 29 novembre 2018
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief executes ASSIGMENT WORD tree
 ** \param node pointer to AST tree
 ** \param fd struct of file descriptor
 **/
void foo_assigment(struct AST *node, __attribute__((unused)) struct fds fd)
{
    if (!node || node->nb_child < 2 || !node->child[0] || !node->child[1])
        return;
    if (node->child[0]->self->name[0] == '$')
    {
        fprintf(
            stderr, "=%s: command not found\n", node->child[1]->self->name);
        node->res = 127;
    }
    else
        setvalue(
            node->child[0]->self->name, getvalue(node->child[1]->self->name));
    node->res = 0;
    set_status(node->res);
}

/**
 ** \brief completes ASSIGMENT WORD tree with name and value
 ** \param str string with the assigment
 ** \param node pointer to the AST to complete
 **/

void fill_assigment(struct AST *node, char *str)
{
    int i = 0;
    for (; str[i] != '='; ++i)
        ;
    str[i] = 0;
    struct Token *left = malloc(sizeof(struct Token));
    struct Token *right = malloc(sizeof(struct Token));
    left->name = copy_str(str);
    left->type = "ASSIGMENT_WORD";
    right->name = copy_str(str + i + 1);
    right->type = "ASSIGMENT_WORD";
    node->child[0] = AST_init(0);
    node->child[1] = AST_init(0);
    node->child[0]->self = left;
    node->child[1]->self = right;
    str[i] = '=';
}

/**
 ** \brief init assigment node
 ** \param token linked list
 ** \return node assigment
 **/

struct AST *assigment_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    struct AST *node = AST_init(2);
    if (!node)
    {
        free(token);
        return NULL;
    }
    token->name = copy_str(
        "="); // FIXME should maybe malloc it to facilitate freeing the tree
    token->type = "ASSIGMENT_WORD";
    node->self = token;
    node->foo = foo_assigment;
    return node;
}

/**
 ** \brief checks if string respects the assigment value format
 ** \param str string to check
 ** \return returns 1 if it is an assigment word, else 0
 **/

int is_assigment(char *str)
{
    if (!str || str[0] == '=' || (str[0] > '0' && str[0] < '9')
        || is_quoted(str))
        return 0;
    int i = 0;
    for (; str[i] != '=' && str[i] != 0; ++i)
        ;
    return (str[i] == '=');
}

/**
 ** \brief check grammar prefix and create prefix node
 ** \param token linked list
 ** \return node prefix
 **/

struct AST *prefix(struct Token **t)
{
    struct AST *redir;
    if (is_assigment(t[0]->name))
    {
        struct AST *assigment_word = assigment_init();
        fill_assigment(assigment_word, t[0]->name);
        *t = t[0]->next;
        return assigment_word;
    }
    else if ((redir = redirection(t)) != NULL)
    {
        return redir;
    }
    else
        return NULL;
}
