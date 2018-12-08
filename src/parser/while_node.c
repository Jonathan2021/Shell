/**
 ** \file parser/while_node.c
 ** \brief all about the rule_while as the grammar (see the subject)
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"
/**
 ** \brief Execution of while node
 ** \param the node while
 ** \param fd a struct of file descriptor
 **/
void foo_while(struct AST *node, struct fds fd)
{
    if (!node || node->nb_child < 2 || !node->child[0] || !node->child[1])
        return;
    node->child[0]->foo(node->child[0], fd);
    node->res = node->child[0]->res;
    while (node->res)
    {
        node->child[1]->foo(node->child[1], fd);
        node->child[0]->foo(node->child[0], fd);
        node->res = node->child[0]->res;
    }
}
/**
 ** \brief initializate the while node but not fill with good node in child
 ** \param token is the chain list of tokens
 ** \return return the while node init
 **/
struct AST *while_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_while;
    token->type = "WHILE";
    // node->child[0] = left_body
    // node->child[1] = right_body
    return node;
}
/**
 ** \brief the rule while from the grammar and crete the associative AST node
 ** \param t  is the chain list of tokens
 ** \return the new node while with all his good child
 **/
struct AST *rule_while(struct Token **t)
{
    struct AST *condition;
    struct AST *do_body;
    struct Token *name;

    struct Token *tmp = *t;
    if (strcmp("while", tmp->name))
        return NULL;
    name = tmp;
    next_token(&tmp);
    if (!(condition = compound_list(&tmp)))
        return NULL;
    if (tmp == NULL)
        tmp = call_ps2(t);

    if (!(do_body = do_group(&tmp)))
    {
        AST_destroy(condition);
        return NULL;
    }
    struct AST *node = while_init(name);
    node->child[0] = condition;
    node->child[1] = do_body;
    *t = tmp;
    return node;
}
