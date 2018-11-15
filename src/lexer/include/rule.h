#pragma once

struct AST *and_or(struct Token **t);
struct AST *else_clause(struct Token **t);
struct AST *rule_if(struct Token **t);
struct AST *list(struct Token **t);
struct AST *input(struct Token **t);
struct AST *shell_command(struct Token **t);