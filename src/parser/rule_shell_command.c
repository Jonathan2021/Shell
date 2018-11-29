/**
 ** \file parser/rule_shell_command.c
 ** \brief All about the shell_comand rule (see the subject)
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include "include/foo.h"
#include "include/my_tree.h"
#include "include/rule.h"
/**
 ** \brief Init a word node with the given first givem token in the list
 ** \param token the chain list of tokens
 ** \return The new AST node for the word fil in it
 **/
struct AST *word_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

void foo_shell_cmd(__attribute__((unused)) struct AST *node,
    __attribute__((unused)) struct fds fd)
{
    return;
}
/**
 ** \brief Init a shell command AST node
 ** \return The new AST node of type shell command not init
 **/
struct AST *shell_cmd_init(void)
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    struct AST *node = AST_init(3);
    if (!node)
    {
        free(token);
        return NULL;
    }
    token->type = "SHELL COMMAND";
    token->name = "SHELL COMMAND";
    node->self = token;
    node->foo = foo_shell_cmd;
    return node;
}

/**
 ** \brief Create and fill the new node of shell command with the good thing as
 *the grammar say,
 ** \param t the chain list oftokens
 ** \return The new AST node of type shell command with the good fill
 **/
struct AST *shell_command(struct Token **t)
{
    struct Token *t1 = *t;
    struct Token *t2 = *t;
    struct Token *ouvre;
    struct Token *ferme;
    struct AST *shell = NULL;
    if (strcmp("{", t1->name) == 0 || strcmp("(", t1->name) == 0)
    {
        ouvre = t1;
        t1 = t1->next;
        if (t1 == NULL)
            return NULL;
        if ((shell = compound_list(&t1)) != NULL)
        {
            if (t1 == NULL)
            {
                // get_token();
                AST_destroy(shell);
            }
            if ((!strcmp("}", t1->name) || !strcmp(")", t1->name))
                && !strcmp(ouvre->name, t1->name))
            {
                ferme = t1;
                t1 = t1->next;
                *t = t1;
                struct AST *shell_node = shell_cmd_init();
                shell_node->child[0] = word_init(ouvre);
                shell_node->child[1] = shell;
                shell_node->child[2] = word_init(ferme);
                return shell_node;
            }
            else
                AST_destroy(shell);
        }
    }
    else if ((shell = rule_for(&t2)) != NULL)
    {
        *t = t2;
    }
    else if ((shell = rule_while(&t2)) != NULL)
    {
        *t = t2;
    }
    else if ((shell = rule_until(&t2)) != NULL)
    {
        *t = t2;
    }
    else if ((shell = rule_case(&t2)) != NULL)
    {
        *t = t2;
    }
    else if ((shell = rule_if(&t2)) != NULL)
    {
        *t = t2;
    }
    else
        return NULL;
    return shell;
}
