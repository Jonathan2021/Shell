#ifndef SHELL_H
#define SHELL_H
#include <time.h>
#include "../parser/include/lexer_struct.h"
struct PS
{
    char *name;
    char *value;
    struct PS *next;
};

extern struct PS *ps;

void add_token(struct Token **token, char *str);
struct Token *parse_path(struct Token *token, char **argv, long argc);
void DestroyToken(struct Token *t);
struct PS *get_ps(void);
void init_ps(void);
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
void writehistory(char *str);
void init_history(void);
int check_rl(void);
int up_arrow();
int down_arrow();
void time_out(clock_t time);
struct Token *lexer(struct Token *t);
char *get_file(char *name);
void set_file(char *name, char *value);
int is_ionumber(char *str);
void reset_file(void);

#endif /* !SHELL_H */
