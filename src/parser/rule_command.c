/**
 ** \file parser/rule_command.c
 ** \brief File for the rule command (see subject)
 ** \date 29 novembre 2018
 **/


#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"
#include <stdlib.h>


/**
 ** \brief Init the node command not fill with child
 ** \return The node command initializied
 **/
struct AST *command_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    token->name = "command";
    token->type = "COMMAND";
    node->self = token;
    return node;
}
/**
 ** \brief Add a new command to the child of the command node
 ** \param cmd is the node command to add the child
 ** \param new is the new node to add  to the child of cmd
 **/
void add_cmd(struct AST *cmd, struct AST *new)
{
    cmd->nb_child++;
    cmd->child = realloc(cmd->child, cmd->nb_child * sizeof(struct AST));
    cmd->child[cmd->nb_child - 1] = new;
}

/**
 ** \brief Rule of the grammar command create the node command wit all the good child
 ** \param t is the chain list of tokens
 ** \return the good command name with his good child
 **/
struct AST *command(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *to_add;
    if (tmp && (to_add = simple_command(&tmp)))
    {
        *t = tmp;
        return to_add;
    }
    struct AST *res = command_init();
    if (tmp && ((to_add = shell_command(&tmp)) || (to_add = funcdec(&tmp))))
    {
        add_cmd(res, to_add);
        *t = tmp;
        while (tmp && (to_add = redirection(&tmp)))
        {
            add_cmd(res, to_add);
            *t = tmp;
        }
        return res;
    }
    AST_destroy(res);
    return NULL;
}

