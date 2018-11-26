#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"

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

struct AST *until_init(struct Token *token)
{
    struct AST *node = AST_init(2);
    if(!node)
        return NULL;
    node->self = token;
    node->foo = foo_until;
    //node->child[0] = left_body
    //node->child[1] = right_body
    return node;
}

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
        call_ps2(t, &tmp);

    if (!(do_body = do_group(&tmp)))
        return NULL;
    //Pas besoin  de tmp->next?
    struct AST *node = until_init(name);
    node->child[0] = condition;
    node->child[1] = do_body;
    *t = tmp;
    return node;
} 
