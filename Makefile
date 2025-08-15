# minishell/Makefile

NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
RL_FLAGS = -lreadline

SRC = ./src/main.c ./src/minishell_loop.c \
./src/utils/str_utils.c ./src/utils/ft_strchr.c ./src/utils/ft_strjoin.c \
./src/utils/ft_printstderr.c ./src/utils/ft_strcpy.c \
./src/utils/ft_strnstr.c ./src/utils/ft_isschar.c ./src/utils/ft_isspace.c \
./src/utils/ft_atoi.c ./src/utils/ft_strcmp.c  ./src/utils/sort_string_array.c\
./src/tokenizer/tokenizer.c \
./src/tokenizer/utils/utils.c \
./src/tokenizer/helper_functions/handle_esc_seq.c ./src/tokenizer/helper_functions/handle_special.c \
./src/tokenizer/helper_functions/handle_word.c ./src/tokenizer/helper_functions/handle_quotes.c \
./src/tokenizer/helper_functions/expand_dollar.c \
./src/parser/parser.c \
./src/parser/parser_utils/is_syntax_error.c ./src/parser/parser_utils/is_redirection.c \
./src/parser/parser_utils/free_ast.c \
./src/executor/executor.c \
./src/executor/path_commands.c \
./src/executor/builtins/builtin_echo.c \
./src/executor/builtins/builtin_pwd.c \
./src/executor/builtins/builtin_cd.c \
./src/executor/builtins/builtin_env.c \
./src/executor/builtins/builtin_export/builtin_export.c \
./src/executor/builtins/builtin_export/case_noargs_export.c \
./src/executor/builtins/builtin_export/case_withargs_export.c \
./src/executor/builtins/builtin_unset.c \
./src/executor/builtins/builtin_exit.c \
./src/executor/execute_node_type/execute_redirin.c \
./src/executor/execute_node_type/execute_redirout.c \
./src/executor/execute_node_type/execute_append.c \
./src/executor/execute_node_type/execute_heredoc.c \
./src/executor/execute_node_type/execute_pipe.c \
./src/executor/executor_utils/is_valid_key.c \
./src/executor/executor_utils/trimquotes.c \
./src/executor/executor_utils/redirection_utils.c \
./src/executor/executor_utils/redirection_input_utils.c \
./src/env/init_env_list.c ./src/env/get_env_value.c ./src/env/update_env.c \
./src/env/remove_env_value.c ./src/env/env_list_size.c ./src/env/free_env_list.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(RL_FLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
