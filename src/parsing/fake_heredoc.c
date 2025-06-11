/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:56:26 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/10 20:02:42 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_delimiter_or_eof(char *line, char *del)
{
	if (!line)
	{
		write(2, "\nminishell: unexpected EOF while looking for delimiter\n", 56);
		set_exit_status(0, 1337);
		return (1);
	}
	if (!ft_strcmp(line, del))
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	write_her_to_file(int fd_write, char *del, int fd_read)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signum == 130)
		{
			close(fd_write);
			close(fd_read);
			free(line);
			set_exit_status(g_signum, 1337);
			return ;
		}
		if (check_delimiter_or_eof(line, del))
			break ;
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
	signal(SIGINT, sigint_handler_her);
	write_her_to_file(fd_write, del, fd_read);
	free(file_name);
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
	g_signum = 0;
	while (copy)
	{
		if (check_pipe(0, copy, prev) || check_redir(0, copy))
			check = 1;
		if (!check)
		{
			open_heredocs(copy);
			if (g_signum == 130)
				return (1);
		}
		if (check)
			return (1);
		prev = copy;
		copy = copy->next;
	}
	return (0);
}
