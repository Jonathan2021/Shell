/**
 ** \file parser/rule_case.c
 ** \brief all about the rule_case as the grammar (see the subject)
 ** \date 29 novembre 2018
 **/
#include <stdio.h>
#include <stdlib.h>
#include "include/my_tree.h"
#include "include/rule.h"
#include "../include/shell.h"


/**
 ** \brief Execution of case node
 ** \param the node case to be exec
 ** \param fd a struct of file descriptor
 **/
void foo_rule_case(struct AST *node, struct fds fd)
{
    if (!node || !node->child[0] || !node->child[1])
        return;
    struct AST *cur_item;
    int nbchild;
    for (int i = 0; i < node->child[1]->nb_child; ++i)
    {
        cur_item = node->child[1]->child[i];
        if (!cur_item)
            return;
        if(cur_item->nb_child < 2)
            continue;
        nbchild = cur_item->nb_child;
        for (int j = 0; j < nbchild - 1; ++j)
        {
            if(!strcmp(getvalue(cur_item->child[j]->self->name), 
                    getvalue(node->child[0]->self->name)))
            {
                if (!strcmp(cur_item->child[nbchild - 1]->self->type,
                        "COMPOUND"))
                {
                    cur_item->child[nbchild - 1]->foo(cur_item->child[nbchild], fd);
                    return;
                }
            }
        }
    }
}
/**
 ** \brief initializate the case node but not fill with good node in child
 ** \return the new case node not with the child
 **/
struct AST *rule_case_init()
{
    struct Token *token = malloc(sizeof(struct Token));
    if(!token)
        return NULL;
    struct AST *node = AST_init(2);
    if(!node)
    {
        free(token);
        return NULL;
    }
    token->type = "CASE";
    token->name = "case";
    node->self = token;
    node->foo = foo_rule_case;
    return node;
}
/**
 ** \brief the rule case from the grammar and crete the associative AST node
 ** \param t  is the chain list of tokens
 ** \return the new node case with all his good child
 **/
struct AST *rule_case(struct Token **t)
{
    struct Token *condition;
    struct AST *case_body = NULL;
    struct Token *tmp = *t;
    if (strcmp(tmp->name,"case"))
        return NULL;
    tmp = tmp->next;
    if (tmp == NULL || strcmp(tmp->type,"WORD"))
        return NULL;
    condition = tmp;
    tmp = tmp->next;
    if (tmp == NULL)
        return NULL;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
            break;
    }
    if (tmp == NULL || strcmp(tmp->name,"in"))
            return NULL;
    tmp = tmp->next;
    if (tmp == NULL)
        return NULL;
    while(strcmp(tmp->name,"\n") == 0)
    {
        tmp = tmp->next;
        if (tmp == NULL)
        {
            return NULL;
        }
    }
    case_body = case_clause(&tmp);
    if (tmp == NULL || strcmp(tmp->name,"esac"))
    {
        if (case_body)
            AST_destroy(case_body);
        return NULL;
    }
    tmp = tmp->next;
    *t = tmp;
    struct AST *node = rule_case_init(); 
    node->child[0] = word_init(condition);
    node->child[1] = case_body;
    return node;
}
