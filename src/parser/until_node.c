/**
 ** \file parser/until_node.c
 ** \brief all about the rule_until as the grammar (see the subject)
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"

/**
 ** \brief Execution of until node
 ** \param the node until
 ** \param fd a struct of file descriptor
 **/
void foo_until(struct AST *node, struct fds fd)
{
    if (!node || node->nb_child < 2 || !node->child[0] || !node->child[1])
    {
        printf("failed first test until\n");
        return;
    }
    node->child[0]->foo(node->child[0], fd);
    printf("child res is : %d\n", node->child[0]->res);
    node->res = !node->child[0]->res;
    while (node->res)
    {
        node->child[1]->foo(node->child[1], fd);
        node->child[0]->foo(node->child[0], fd);
        node->res = !node->child[0]->res;
    }
}
/**
 ** \brief initializate the until node but not fill with good node in child
 ** \param token is the chain list of tokens
 ** \return return the until node init
 **/
struct AST *until_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if (!node)
        return NULL;
    node->self = token;
    node->foo = foo_until;
    token->type = "UNTIL";
    return node;
}
/**
 ** \brief the rule until from the grammar and crete the associative AST node
 ** \param t  is the chain list of tokens
 ** \return the new node until with all his good child
 **/
struct AST *rule_until(struct Token **t)
{
    struct AST *condition;
    struct AST *do_body;
    struct Token *name;

    struct Token *tmp = *t;
    if (strcmp("until", tmp->name))
        return NULL;
    name = tmp;
    next_token(&tmp);
    if (!(condition = compound_list(&tmp)))
        return NULL;
    if (tmp == NULL)
        tmp = call_ps2(t);

    if (!(do_body = do_group(&tmp)))
        return NULL;
    // Pas besoin  de tmp->next?
    struct AST *node = until_init(name);
    node->child[0] = condition;
    node->child[1] = do_body;
    *t = tmp;
    return node;
}
