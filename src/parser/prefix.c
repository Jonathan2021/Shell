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

void foo_assigment(struct AST *node, __attribute__((unused))struct fds fd)
{
    if(!node || node->nb_child < 2 || !node->child[0] || !node->child[1])
        return;
    setvalue(node->child[0]->self->name, 
            getvalue(node->child[1]->self->name));

    
}

void fill_assigment(struct AST *node, char *str)
{
    int i = 0;
    for(; str[i] != '='; ++i);
    str[i] = 0;
    struct Token *left = malloc(sizeof(struct Token));
    struct Token *right = malloc(sizeof(struct Token));
    left->name = str;
    left->type = "WORD";
    right->name = str +  i + 1;
    right->type = "WORD";
    node->child[0] = AST_init(0);
    node->child[1] = AST_init(0);
    node->child[0]->self = left;
    node->child[1]->self = right;
    //Risque de poser probleme quand on free puisque free left revient à free right
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
    if(!node)
    {
        free(token);
        return NULL;
    }
    token->name = "="; //FIXME should maybe malloc it to facilitate freeing the tree
    token->type = "ASSIGMENT_WORD";
    node->self = token;
    node->foo = foo_assigment;
    return node;
}
/**
 ** \brief check grammar prefix and create prefix node
 ** \param token linked list
 ** \return node prefix
 **/

int is_assigment(char *str)
{
    if (!str || str[0] == '=' || (str[0] > '0' && str[0] < '9'))
        return 0;
    int i = 0;
    for (; str[i] != '=' && str[i] != 0; ++i);
    return (str[i] == '=');
}

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
