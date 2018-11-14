#ifndef SHELL_H
# define SHELL_H

struct token
{
    char *name;
    char *type;
    struct token *next;
};

void add_token(struct token **token, char *str);
struct token *parse_path(struct token *token, char * str);

#endif /* !SHELL_H */
