#ifndef SHELL_H
# define SHELL_H

struct Token
{
    char *name;
    char *type;
    struct Token *next;
};

void add_token(struct Token **token, char *str);
struct Token *parse_path(struct Token *token, char * str);

#endif /* !SHELL_H */
