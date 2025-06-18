/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:49:34 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/16 16:37:47 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	open_heredoc_file(char *file_name, int *fd_read, int *fd_write)
{
	*fd_write = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*fd_write == -1)
	{
		perror("open for write");
		set_exit_status(1, 1337);
		return (-1);
	}
	*fd_read = open(file_name, O_RDONLY);
	if (*fd_read == -1)
	{
		close(*fd_write);
		perror("open for write");
		set_exit_status(1, 1337);
		return (-1);
	}
	unlink(file_name);
	return (0);
}

void	handle_all_herdocs(t_exec *execs, t_env *env)
{	
	t_redir		*redir;

	signal(SIGINT, sigint_handler_her);
	while (execs)
	{
		redir = execs->redirections;
		while (redir)
		{
			if (redir->heredoc_count > 16)
			{
				write(2, "minishell: maximum here-document \
				count exceeded\n", 49);
				exit(2);
			}
			if (redir->type == HEREDOC && handle_heredoc(redir, env) == -1)
				return ;
			redir = redir->next;
		}
		execs = execs->next;
	}
}
