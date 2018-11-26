#ifndef LEXER_STRUCT_H
#define LEXER_STRUCT_H

struct Token
{
    char *name;
    char *type;
    struct Token *next;
};

//void add_token(struct Token **token, char *str);
//struct Token *parse_path(struct Token *token, char * str);

//int input(struct Token **t);
//int list(struct Token **t);
//int and_or(struct Token **t);
//int rule_if (struct Token **t);
//int else_clause(struct Token **t);
//int shell_command(struct Token **t);

#endif /* ! LEXER_STRUCT_H */
