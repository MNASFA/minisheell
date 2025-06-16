/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:39:02 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:24:30 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	handle_input_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_in(red->filename, &(*commands)->infd);
	if (i == -3)
	{
		perror(red->filename);
		return (-1);
	}
	if (isatty(i))
	{
		close(i);
		(*commands)->infd = -2;
	}
	else
		(*commands)->infd = i;
	return (0);
}

static int	handle_output_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_out(red->filename, &(*commands)->outfd, red);
	if (i == -3)
	{
		perror(red->filename);
		return (-1);
	}
	if (isatty(i))
	{
		close(i);
		(*commands)->outfd = -2;
	}
	else
		(*commands)->outfd = i;
	return (0);
}

static int	handle_heredoc_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_out_her(red, &(*commands)->infd);
	if (i == -3)
		return (-1);
	(*commands)->infd = i;
	return (0);
}

int	open_in_out(t_exec **commands)
{
	t_redir	*red;

	if (!commands || !(*commands))
		return (-1);
	red = (*commands)->redirections;
	while (red)
	{
		if (red->type == REDIR_IN)
		{
			if (handle_input_redir(red, commands) == -1)
				return (-1);
		}
		else if (red->type == REDIR_OUT || red->type == APPEND)
		{
			if (handle_output_redir(red, commands) == -1)
				return (-1);
		}
		else if (red->type == HEREDOC)
		{
			if (handle_heredoc_redir(red, commands) == -1)
				return (-1);
		}
		red = red->next;
	}
	return (0);
}
