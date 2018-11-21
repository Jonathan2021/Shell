#ifndef SHELL_H
# define SHELL_H
#include "../lexer/include/lexer_struct.h"
struct PS
{
    char *name;
    char *value;
    struct PS *next;
};

void add_token(struct Token **token, char *str);
struct Token *parse_path(struct Token *token, char **argv, long argc
    ,struct PS *ps);
void set_value(char *name, char *value, struct PS **ps);
char *get_value(char *name, struct PS *ps);
void reset_value(struct PS *ps);
long str_to_argv(char **argv, char *str);
struct Token *lexer(struct Token *t);
int check_option(struct Token *token, struct PS *ps);
struct Token *create_token(struct Token *token, char *str);
struct PS *init_ps(void);
struct Token *read_file(char *file, struct Token *token);

#endif /* !SHELL_H */
