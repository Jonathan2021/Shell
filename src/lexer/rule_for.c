#include "include/lexer_struct.h"
#include "my_tree.h"
#include "rule.h"
#include <stdlib.h>

struct AST *for_init(struct Token *token)
{
    struct AST *node = AST_init(3);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

struct AST *in_init(struct Token *token)
{
    struct AST *node = AST_init(0);
    if (!node)
        return NULL;
    node->self = token;
    return node;
}

void add_in(struct AST *in, struct Token *token)
{
    struct AST *new = word_init(token);
    in->nb_child++;
    in->child = realloc(in->child, in->nb_child * sizeof(struct AST));
    in->child[in->nb_child - 1] = new;
}

struct AST *rule_for(struct Token **t)
{
    struct AST *for_node;
    struct AST *token;
    struct Token *tmp = *t;
    struct AST *do_group;
    struct Token *checkpoint;
    if (!tmp || strcmp(tmp->name,"for"))
        return NULL;
    tmp = tmp->next;
    if (!tmp || strcmp(tmp->type,"WORD"))
        return NULL;
    for_node = for_init(token);
    for_node->child[0] = word_init(tmp);
    tmp = tmp->next;
    checkpoint = tmp;
    if (!strcmp(tmp->name,";"))
    {
        for_node->child[1] = word_init(tmp);
        tmp = tmp->next;
        checkpoint = tmp;
    }
    else
    {
        struct AST *in;
        while(tmp && !strcmp(tmp->name,"\n"))
        {
            tmp = tmp->next;
        }
        if (tmp && !strcmp(tmp->name,"in"))
        {
           in = in_init(tmp); 
            tmp = tmp->next;
            
            while(tmp && !strcmp(tmp->type,"WORD"))
            {
                add_in(in, tmp);
                tmp = tmp->next;
            }
            if (tmp && (!strcmp(tmp->name,";") ||
                !strcmp(tmp->name,"\n")))
                {
                    tmp = tmp->next;
                    checkpoint = tmp;
                    for_node->child[1] = in;
                }
        }
    }
    tmp = checkpoint;
    while(tmp && !strcmp(tmp->name,"\n"))
    {
        tmp = tmp->next;
    }
    if(!tmp || !(do_group = do_group(&tmp)))
    {
        AST_destroy(for_node);
        return NULL;
    }
    for_node->child[2] = do_group;
    *t = tmp;
    return for_node;
}
