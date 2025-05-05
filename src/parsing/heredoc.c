/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:49:34 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/30 10:35:55 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_name(int fd)
{
	char	*file_name;
	int		count;
	char	read_char;
	
	file_name = malloc(NAME_LEN + 1);
	if (!file_name)
	{
		close(fd);
		printf("FAILED MALLOC");
		return (NULL);
	}
	count = 0;
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

char	*generate_filename()
{
	int	fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
	{
		perror("failed open '/dev/random'");	
		return (NULL);
	}
	return (get_name(fd));
}

int	handle_heredoc(t_exec *exec, t_env *env)
{
	char	*line;
	int		fd_write;
	int		fd_read;
	char	*file_name;
	char	*expand_line;
	
	if (!exec->delimiter || !exec->heredoc)
		return (-1);
	file_name = generate_filename();
	fd_read = open(file_name , O_RDONLY);
	fd_write = open(file_name , O_CREAT | O_WRONLY | O_TRUNC, 0644);
	unlink(file_name);
	if (fd_write < 0)
	{
		perror("open for write");
		free(file_name);
		exit(1);
	}
	
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nminishell: unexpected EOF while looking for delimiter\n");
			break;
		}
		if (ft_strcmp(exec->delimiter, line) == 0)
		{
			free(line);
			break ;
		}
		expand_line = expand_herdoc_variables(line, env, 0, 0);
		write(fd_write, expand_line, ft_strlen(expand_line));
		write(fd_write, "\n", 1);
		free(line);
	}
	close(fd_write);
	free(file_name);
	return (fd_read);
}

void	handle_all_herdocs(t_exec *execs, t_env *env)
{	
	while (execs)
	{
		if (execs->heredoc && execs->delimiter)
			execs->herdoc_fd = handle_heredoc(execs, env);
		else
			execs->herdoc_fd = -1;
		execs = execs->next;
	}
}