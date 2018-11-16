#include <stdlib.h>
#include "lexer_struct.h"
#include "my_tree.h"
#include "rule.h"

struct AST *command_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if (!token)
        return NULL;
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    token->name = "command"
    token->type = "COMMAND"
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
        *t = tmp;
        add_cmd(res, to_add);
        return res;
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
        return res;
    }
    AST_destroy(res);
    return NULL;
}
