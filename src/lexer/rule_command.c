#include "include/lexer_struct.h"
#include "include/my_tree.h"
#include "include/rule.h"
#include <stdlib.h>

struct AST *command_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

void add_cmd(struct AST *in, struct AST *new)
{
    in->nb_child++;
    in->child = realloc(in->child, in->nb_child * sizeof(struct AST));
    in->child[in->nb_child - 1] = new;
}

struct AST *command(struct Token **t)
{
    struct Token *tmp = *t;
    struct AST *cmd = command_init(*t);
    if ((cmd = simple_command(&tmp)) != NULL)
    {
        *t = tmp;
        return cmd;
    }
    tmp = *t;
    if ((cmd = shell_command(&tmp)) != NULL)
    {
        *t = tmp;
        if (tmp == NULL)
            return cmd;
        while (1)
        {
            struct AST *red = command_init(*t);
            if ((red = redirection(&tmp)) != NULL)
            {
                add_cmd(cmd, red);
                *t = tmp;
            }
            else
                return cmd;
            if (tmp == NULL)
                return cmd;
        }
    }
    tmp = *t;
    if ((cmd = funcdec(&tmp)) != NULL)
    {
        *t = tmp;
        if (tmp == NULL)
            return cmd;
        while (1)
        {
            struct AST *red = command_init(*t);
            if ((red = redirection(&tmp)) != NULL)
            {
                add_cmd(cmd, red);
                *t = tmp;
            }
            else
                return cmd;
            if (tmp == NULL)
                return cmd;
        }
    }
    return NULL;

}
