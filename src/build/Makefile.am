AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = 42sh

42sh_SOURCES = src/main.c \
	./src/lexer/and_node.c \
	./src/lexer/if_node.c \
	./src/lexer/rule_list.c \
	./src/lexer/rule_input.c \
	./src/lexer/else_node.c \
	./src/lexer/word_node.c \
	./src/lexer/my_tree.c \
	./src/print_ast/print_ast.c

42sh_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
