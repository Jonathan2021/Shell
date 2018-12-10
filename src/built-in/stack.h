#pragma once

enum type {
    TYPE_INT = 0,
    TYPE_STR = 10,
};

union data{
    int res;
    char *str;
};

struct full_data
{
    enum type type;
    union data data;
};

struct stack {
    struct full_data fd;
    struct stack *next;
};

struct tree {
    struct tree *left;
    struct right *right;
    struct full_data fd;
};

struct stack *stack_init(void);
struct stack *push(struct stack *st, struct full_data enter);
struct full_data top(struct stack *st);
void stack_destroy(struct stack *st);
struct stack *pop(struct stack *st);
void print(struct stack *st);
struct stack *stack_cat(struct stack *st1, struct stack *st2);
