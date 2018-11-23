CC = gcc
CFLAGS = -std=c99 -pedantic -Wextra -Wall -g -lreadline
SRC = src/lexer/and_node.c \
	src/lexer/rule_case_clause.c \
	src/lexer/bang_node.c \
	src/lexer/lessand_node.c \
	src/lexer/rule_command.c \
	src/lexer/case_node.c \
	src/lexer/lessgreat_node.c \
	src/lexer/rule_compound_list.c \
	src/lexer/dgreat_node.c \
	src/lexer/rule_for.c \
	src/lexer/dlessdash_node.c \
	src/main.c \
	src/file/tool.c \
	src/file/history.c \
	src/print_ast/print_ast.c \
	src/lexer/rule_funcdec.c \
	src/lexer/dless_node.c \
	src/lexer/my_tree.c \
	src/lexer/rule_input.c \
	src/lexer/do_group.c \
	src/lexer/rule_list.c \
	src/lexer/element.c \
	src/lexer/rule_redirection.c \
	src/lexer/else_node.c \
	src/lexer/pipe_node.c \
	src/lexer/until_node.c \
	src/lexer/greatand_node.c \
	src/lexer/prefix.c \
	src/lexer/while_node.c \
	src/lexer/if_node.c \
	src/lexer/rule_case.c \
	src/lexer/rule_shell_command.c \
	src/lexer/rule_simple_command.c
EXEC = 42SH

all: 
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)


clean:
	rm $(EXEC) 
