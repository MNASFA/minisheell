/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:49:34 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/16 20:06:39 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

int g_signum = 0;

static char	*get_name(int fd, int count)
{
	char	*file_name;
	char	read_char;

	file_name = malloc(NAME_LEN + 1);
	if (!file_name)
	{
		close(fd);
		return (NULL);
	}
	while (count < NAME_LEN)
	{
		if (read(fd, &read_char, 1) < 0)
		{
			close(fd);
			perror("fail read\n");
			return (NULL);
		}
		if (ft_isalnum(read_char))
			file_name[count++] = read_char;
	}
	file_name[NAME_LEN] = '\0';
	close(fd);
	return (file_name);
}

char	*generate_filename(void)
{
	int	fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		perror("failed open '/dev/random'");
		return (NULL);
	}
	return (get_name(fd, 0));
}

static int	open_heredoc_file(char *file_name, int *fd_read, int *fd_write)
{
	*fd_read = open(file_name, O_RDONLY);
	*fd_write = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	unlink(file_name);
	if (*fd_write < 0)
	{
		perror("open for write");
		free(file_name);
		exit(set_exit_status(1, 1337));
	}
	return (0);
}

static void	process_heredoc_line(char *line, int quoted, int fd, t_env *env)
{
	char	*expanded;

	if (quoted == 0)
		expanded = expand_herdoc_variables(line, env, 0, 0);
	else
		expanded = line;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	if (quoted == 0)
		free(expanded);
}

void	sigint_handler1(int sig)
{
	(void) sig;
	g_signum = 130;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}
int	handle_heredoc(t_redir *redir, t_env *env)
{
	char	*line;
	char	*file_name;
	int		fd_read;
	int		fd_write;

	if (!redir->delimiter || !redir->is_herdoc)
		return (-1);
	file_name = generate_filename();
	if (!file_name || open_heredoc_file(file_name, &fd_read, &fd_write) < 0)
		return (-1);
	while (1)
	{
		signal(SIGINT, sigint_handler1);
		if (g_signum == 130)
			break ;
		line = readline("> ");
		if (!line)
			return (printf("\nminishell: unexpected EOF\n"), 0);
		if (ft_strcmp(redir->delimiter, line) == 0)
			break ;
		process_heredoc_line(line, redir->quoted_flag, fd_write, env);
		free(line);
	}
	close(fd_write);
	redir->herdoc_fd = fd_read;
	redir->filename = file_name;
	return (0);
}

void	handle_all_herdocs(t_exec *execs, t_env *env)
{	
	t_redir		*redir;

	while (execs)
	{
		redir = execs->infiles;
		while (redir)
		{ 
			if (redir->heredoc_count > 16)
			{
				printf("minishell: maximum here-document count exceeded\n");
				exit(2);
			}
			if (redir->is_herdoc)
				handle_heredoc(redir, env);
			redir = redir->next;
		}
		execs = execs->next;
	}
}
