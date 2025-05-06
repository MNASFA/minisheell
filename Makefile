NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
HEADER = minishell.h
SRCS = src/main.c src/utils/ft_split.c src/utils/utils.c src/parsing/tokenizer.c src/parsing/expand.c \
		src/parsing/commands.c src/parsing/syntax_error.c src/parsing/prepare_commands.c src/parsing/free_utils.c src/parsing/heredoc.c \
		src/parsing/init_vars.c src/parsing/envirement.c src/parsing/expand_heredoc.c 
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