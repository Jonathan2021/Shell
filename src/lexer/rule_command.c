#include <stdlib.h>
#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"

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

void add_cmd(struct AST *cmd, struct AST *new)
{
    cmd->nb_child++;
    cmd->child = realloc(cmd->child, cmd->nb_child * sizeof(struct AST));
    cmd->child[cmd->nb_child - 1] = new;
}

struct AST *command(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *res = command_init();
    struct AST *to_add;
    if (tmp && (to_add = simple_command(&tmp)))
    {
        free_l(res);
        *t = tmp;
        return to_add;
    }
    if (tmp && ((to_add = shell_command(&tmp)) || (to_add = funcdec(&tmp))))
    {
        add_cmd(res, to_add);
        *t = tmp;
        while (tmp && (to_add = redirection(&tmp)))
        {
            add_cmd(res, to_add);
            *t = tmp;
        }
        if (res->nb_child == 1)
        {
            to_add = res->child[0];
            free_l(res);
            return to_add;
        }
        return res;
    }
    AST_destroy(res);
    return NULL;
}
