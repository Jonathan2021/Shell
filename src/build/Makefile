CC = gcc
CFLAGS = -std=c99 -pedantic -Wextra -Wall -g 
SRC = src/parser/and_node.c \
	src/parser/rule_case_clause.c \
	src/parser/bang_node.c \
	src/parser/lessand_node.c \
	src/parser/rule_command.c \
	src/parser/case_node.c \
	src/parser/lessgreat_node.c \
	src/parser/rule_compound_list.c \
	src/parser/dgreat_node.c \
	src/parser/rule_for.c \
	src/parser/dlessdash_node.c \
	src/main.c \
	src/file/tool_file.c \
	src/lexer/lexer.c \
	src/file/history.c\
	src/print_ast/print_ast.c \
	src/file/tool.c \
	src/parser/rule_funcdec.c \
	src/parser/dless_node.c \
	src/parser/my_tree.c \
	src/parser/rule_input.c \
	src/parser/do_group.c \
	src/parser/rule_list.c \
	src/parser/element.c \
	src/parser/rule_redirection.c \
	src/parser/else_node.c \
	src/parser/pipe_node.c \
	src/parser/until_node.c \
	src/parser/greatand_node.c \
	src/parser/prefix.c \
	src/parser/while_node.c \
	src/parser/if_node.c \
	src/parser/rule_case.c \
	src/parser/rule_shell_command.c \
	src/parser/foo_token.c \
	src/parser/rule_simple_command.c
EXEC = 42SH

all: 
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) -lreadline


clean:
	rm $(EXEC) 
