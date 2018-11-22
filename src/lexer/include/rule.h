#pragma once
#include <string.h>
#include "my_tree.h"
#include "lexer_struct.h"

struct AST *and_or(struct Token **t);
struct AST *else_clause(struct Token **t);
struct AST *rule_if(struct Token **t);
struct AST *list(struct Token **t);
struct AST *input(struct Token **t);
struct AST *shell_command(struct Token **t);
struct AST *pipeline(struct Token **t);
struct AST *command(struct Token **t);
struct AST *simple_command(struct Token **t);
struct AST *funcdec(struct Token **t);
struct AST *redirection(struct Token **t);
struct AST *prefix(struct Token **t);
struct AST *element(struct Token **t);
struct AST *compound_list(struct Token **t);
struct AST *rule_for(struct Token **t);
struct AST *rule_while(struct Token **t);
struct AST *rule_until(struct Token **t);
struct AST *rule_case(struct Token **t);
struct AST *do_group(struct Token **t);
struct AST *case_clause(struct Token **t);
struct AST *case_item(struct Token **t);
struct AST *word_init(struct Token *token);
struct AST *while_init(struct Token *token);
void add_case(struct AST *case_clause, struct AST *case_item);
struct AST *case_init(void);
void setvalue(char *, char *);
char *getvalue(char *);
