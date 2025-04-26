/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:49:34 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/26 18:20:18 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*generate_filename()
{
	static int	herdoc_counter;
	char		*num_str;
	char		*filename;

	num_str = ft_itoa(++herdoc_counter);
	filename = ft_strjoin(".herdoc_", num_str);
	free(num_str);
	return (filename);
}

int	handle_heredoc(t_exec *exec)
{
	char	*line;
	int		fd_write;
	int		fd_read;
	char	*file_name;
	
	if (!exec->delimiter || !exec->heredoc)
		return (-1);
	file_name = generate_filename();
	fd_write = open(file_name , O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 1);
		free(line);
	}
	close(fd_write);
	
	fd_read = open(file_name , O_RDONLY, 0644);
	if (fd_read < 0)
	{
		perror("open for read");
		free(file_name);
		exit(1);		
	}
	
	add_infile(exec, file_name);
	// unlink(file_name);
	free(file_name);
	return (fd_read);
}

void	handle_all_herdocs(t_exec *execs)
{	
	while (execs)
	{
		if (execs->heredoc && execs->delimiter)
			execs->herdoc_fd = handle_heredoc(execs);
		else
			execs->herdoc_fd = -1;
		execs = execs->next;
	}
}