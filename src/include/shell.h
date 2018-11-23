#ifndef SHELL_H
# define SHELL_H
#include "../lexer/include/lexer_struct.h"
struct PS
{
    char *name;
    char *value;
    struct PS *next;
};

extern struct PS *ps;

void add_token(struct Token **token, char *str);
struct Token *parse_path(struct Token *token, char **argv, long argc);
void set_value(char *name, char *value);
char *get_value(char *name);
void reset_value(void);
long str_to_argv(char **argv, char *str);
struct Token *lexer(struct Token *t);
int check_option(struct Token *token);
struct Token *create_token(struct Token *token, char *str);
struct PS *get_ps(void);
struct Token *read_file(char *file, struct Token *token);
char *getvalue(char *name);
void delete_history(void);
void write_history(char *str);

#endif /* !SHELL_H */
