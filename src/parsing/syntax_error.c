/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:20:40 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/15 10:15:06 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	track_quotes(char *input)
{
	int	in_double;
	int	in_single;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (input[i])
	{
		quotes_state(input[i], &in_single, &in_double);
		i++;
	}
	if (in_single || in_double)
		return (1);
	return (0);
}

char	*check_unclosed_quotes(char *input)
{
	int	track;

	track = track_quotes(input);
	if (track != 0)
	{
		printf("minishell: syntax error unclosed quotes\n");
		return (NULL);
	}
	return (input);
}

int	is_redir(t_token *tokens)
{
	return (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
		|| tokens->type == HEREDOC || tokens->type == APPEND);
}

int	check_redir(int flag, t_token *tokens)
{
	if (!is_redir(tokens))
		return (0);
	if (!tokens->next)
	{
		if (flag)
			printf("minishell: syntax error near unexpected token\n");
		return (1);
	}
	if (is_redir(tokens->next)
		|| (tokens->next->type != WORD
			&& tokens->next->type != HEREDOC_DELIMITER))
	{
		if (flag)
			printf("minishell: syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

int	check_pipe(int flag, t_token *tokens, t_token *prev)
{
	if (tokens->type == PIPE)
	{
		if (!prev || !tokens->next || tokens->next->type == PIPE)
		{
			if (flag)
				printf("minishell: syntax error near unexpected token '|'\n");
			return (1);
		}
	}
	return (0);
}

static void	write_her_to_file(int fd_write, char *del)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nminishell: unexpected EOF while looking for delimiter\n");
			break ;
		}
		if (!ft_strcmp(line, del))
		{
			free(line);
			break ;
		}
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 2);
		free(line);
	}
}

int	fake_heredoc(char *del)
{
	int		fd_read;
	int		fd_write;
	char	*file_name;

	file_name = generate_filename();
	if (!file_name)
		return (-1);
	fd_read = open(file_name, O_RDONLY);
	fd_write = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	unlink(file_name);
	write_her_to_file(fd_write, del);
	close(fd_write);
	return (fd_read);
}

void	open_heredocs(t_token *tokens)
{
	char	*del;
	t_token	*next;

	del = NULL;
	next = tokens->next;
	if (next && next->type == HEREDOC_DELIMITER && tokens->type == HEREDOC)
	{
		del = ft_strdup(next->value);
		close(fake_heredoc(del));
		free(del);
	}
}

int	double_check_errors(t_token *tokens)
{
	int		check;
	t_token	*copy;
	t_token	*prev;

	check = 0;
	copy = tokens;
	prev = NULL;
	while (copy)
	{
		if (check_pipe(0, copy, prev) || check_redir(0, copy))
			check = 1;
		if (!check)
			open_heredocs(copy);
		if (check)
			return (1);
		prev = copy;
		copy = copy->next;
	}
	return (0);
}

int	check_errors(t_token *tokens, int check, t_token *prev, int her_count)
{
	t_token	*copy;

	copy = tokens;
	while (copy)
	{
		if (copy->type == HEREDOC)
			her_count++;
		if (her_count > 16)
		{
			printf("minishell: maximum here-document count exceeded\n");
			exit(2);
		}
		if (check_pipe(1, copy, prev) || check_redir(1, copy))
			check = 1;
		if (check)
		{
			double_check_errors(tokens);
			return (1);
		}
		prev = copy;
		copy = copy->next;
	}
	return (0);
}
