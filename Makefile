NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
LDFLAGS = -lreadline
HEADER = minishell.h
SRCS = src/main.c src/utils/utils.c src/parsing/tokenizer.c src/parsing/expand.c \
		src/parsing/commands.c src/parsing/syntax_error.c src/parsing/prepare_commands.c src/parsing/free_utils.c src/parsing/free_utils2.c src/parsing/heredoc.c \
		src/parsing/envirement.c src/parsing/expand_heredoc.c src/parsing/heredoc2.c src/execution/exit_status.c src/parsing/env_creation.c src/signals.c\
		src/parsing/expand_utils.c src/parsing/fake_heredoc.c src/parsing/parse_command.c src/parsing/parse_help.c src/parsing/build_exec.c\
		# src/execution/execution.c src/execution/spliiit.c src/built_in/cd.c src/built_in/echo.c src/built_in/exit.c \
		# src/built_in/export.c src/built_in/pwd.c src/built_in/env.c src/built_in/unset.c 
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o : %.c ${HEADER}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all