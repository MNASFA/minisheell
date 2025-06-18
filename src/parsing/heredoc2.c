/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:48:52 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 11:18:47 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	process_heredoc_line(char *line, int quoted, int fd, t_env *env)
{
	char	*expanded;

	if (quoted == 0)
		expanded = expand_herdoc_variables(line, env);
	else
		expanded = line;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	if (quoted == 0)
		free(expanded);
	free(line);
}

static int	handle_heredoc_eof(char *file_name, int fd_write,
		int fd_read, t_redir *redir)
{
	write(2, "\nminishell: unexpected EOF while looking for delimiter\n", 56);
	close(fd_write);
	close(fd_read);
	free(file_name);
	redir->herdoc_fd = -1;
	return (set_exit_status(0, 1337), 0);
}

static int	handle_her_signal(int fd_read, int fd_write
	, char *file_name, char *line)
{
	close(fd_read);
	set_exit_status(g_signum, 1337);
	free(file_name);
	free(line);
	close(fd_write);
	return (-1);
}

static int	init_heredoc_file(t_redir *redir, int *fd_read
	, int *fd_write, char **file_name)
{
	if (!redir->delimiter || redir->type != HEREDOC)
		return (-1);
	*file_name = generate_filename();
	if (!*file_name || open_heredoc_file(*file_name, fd_read, fd_write) == -1)
		return (free(*file_name), -1);
	return (0);
}

int	handle_heredoc(t_redir *redir, t_env *env)
{
	char	*line;
	char	*file_name;
	int		fd_read;
	int		fd_write;

	if (init_heredoc_file(redir, &fd_read, &fd_write, &file_name) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (g_signum == 130)
			return (handle_her_signal(fd_read, fd_write, file_name, line));
		if (!line)
			return (handle_heredoc_eof(file_name, fd_write, fd_read, redir));
		if (ft_strcmp(redir->delimiter, line) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, redir->quoted_flag, fd_write, env);
	}
	close(fd_write);
	free(file_name);
	redir->herdoc_fd = fd_read;
	return (0);
}
