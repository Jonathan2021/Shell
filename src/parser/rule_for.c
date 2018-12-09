/**
 ** \file parser/rule_for.c
 ** \brief all about the rule_for as the grammar (see the subject)
 ** \date 29 novembre 2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include "../include/shell.h"
#include "include/foo.h"
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"
/**
 ** \brief Execution of for node
 ** \param the node for
 ** \param fd a struct of file descriptor
 **/
void foo_for(struct AST *node, struct fds fd)
{
    if (!node || node->nb_child < 3 || !node->child[0])
        return;
    if (node->child[1] && strcmp(node->child[1]->self->name, "in"))
        return;
    for (int i = 0; i < node->child[1]->nb_child; ++i)
    {
        setvalue(node->child[0]->self->name,
            getvalue(node->child[1]->child[i]->self->name));
        node->child[2]->foo(node->child[2], fd);
        node->res = node->child[2]->res;
    }
}
/**
 ** \brief initializate the for node but not fill with good node in child
 ** \param token is the chain list of tokens
 ** \return return the for node init
 **/
struct AST *for_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_for;
    return node;
}
/**
 ** \brief initializate the in node not fill with good node in child
 ** \param token is the chain list of tokens
 ** \return the node in of the AST with not the good node in child
 **/

struct AST *in_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

/**
 ** \brief add a new node to the child of the node in
 ** \param in the node AST of type in
 ** \param token is the chain list of tokens
 ** \return the node in of the AST with not the good node in child
 **/
void add_in(struct AST *in, struct Token *token)
{
    struct AST *new = word_init(token);
    in->nb_child++;
    in->child = realloc(in->child, in->nb_child * sizeof(struct AST *));
    in->child[in->nb_child - 1] = new;
}
/**
 ** \brief the rule for from the grammar and crete the associative AST node
 ** \param t  is the chain list of tokens
 ** \return the new node for with all his good child
 **/
struct AST *rule_for(struct Token **t)
{
    struct AST *for_node;
    struct Token *tmp = *t;
    struct AST *do_group_ast;
    struct Token *checkpoint;
    if (!tmp || strcmp(tmp->name, "for"))
        return NULL;
    next_token(&tmp);
    if (!tmp || strcmp(tmp->type, "WORD"))
        return NULL;
    for_node = for_init(*t);
    for_node->child[0] = word_init(tmp);
    next_token(&tmp);
    checkpoint = tmp;
    if (!strcmp(tmp->name, ";"))
    {
        for_node->child[1] = word_init(tmp);
        tmp = tmp->next;
        checkpoint = tmp;
    }
    else
    {
        struct AST *in;
        while (tmp && !strcmp(tmp->name, "\n"))
        {
            next_token(&tmp);
        }
        if (tmp && !strcmp(tmp->name, "in"))
        {
            in = in_init(tmp);
            next_token(&tmp);
            while (tmp && !strcmp(tmp->type, "WORD"))
            {
                add_in(in, tmp);
                next_token(&tmp);
            }
            if (tmp && (!strcmp(tmp->name, ";") || !strcmp(tmp->name, "\n")))
            {
                next_token(&tmp);
                checkpoint = tmp;
                for_node->child[1] = in;
            }
        }
    }
    tmp = checkpoint;
    while (tmp && !strcmp(tmp->name, "\n"))
    {
        next_token(&tmp);
    }
    if (!tmp || !(do_group_ast = do_group(&tmp)))
    {
        AST_destroy(for_node);
        return NULL;
    }
    for_node->child[2] = do_group_ast;
    *t = tmp;
    return for_node;
}
