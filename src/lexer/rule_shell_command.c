#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"




struct AST *word_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if(!node)
        return NULL;
    node->self = token;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

void foo_shell_cmd(__attribute__((unused)) struct AST *node, __attribute__((unused))struct fds fd)
{
    return;
}

struct AST *shell_cmd_init(void)
{
    struct Token *token = malloc(sizeof(struct Token));
    if(!token)
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
//struct AST *assignment_word_init(struct Token *token)
//{

//}

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
        next_token(&t1);
        if ((shell = compound_list(&t1)) != NULL)
        {
            if (t1 == NULL)
            {
                call_ps2(t,&t1);
            }
            if ((!strcmp("}", t1->name) &&  !strcmp(ouvre->name, "{"))
                    || (!strcmp(")", t1->name) && !strcmp(ouvre->name, "(")))
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
