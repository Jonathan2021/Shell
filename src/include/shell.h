#ifndef SHELL_H
# define SHELL_H
/*
struct Token
{
    char *name;
    char *type;
    struct Token *next;
};*/
#include "../lexer/include/lexer_struct.h"
void add_token(struct Token **token, char *str);
struct Token *parse_path(struct Token *token, char **argv, long argc);
void set_value(char *name, char *value);
char *get_value(char *name);
void reset_value(void);
long str_to_argv(char **argv, char *str);
struct Token *lexer(struct Token *t);
int check_option(struct Token *token);
void read_isatty(void);

#endif /* !SHELL_H */