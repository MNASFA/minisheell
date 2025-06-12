/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:36:10 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:25:55 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	open_in(char *filename, int *i)
{
	int	k;

	if (!filename)
		return (-3);
	if (*i > 2)
		close(*i);
	k = open(filename, O_RDONLY);
	if (k == -1)
	{
		set_exit_status(1, 1337);
		return (-3);
	}
	return (k);
}

int	open_out(char *filename, int *i, t_redir *red)
{
	int	k;

	if (!filename || !red)
		return (-3);
	if (*i > 2)
		close(*i);
	if (red->type == REDIR_OUT)
		k = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (red->type == APPEND)
		k = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-3);
	if (k == -1)
	{
		set_exit_status(1, 1337);
		return (-3);
	}
	return (k);
}

int	open_out_her(t_redir *red, int *i)
{
	int	k;

	if (!red)
		return (-3);
	if (*i == -2)
		k = red->herdoc_fd;
	else
	{
		if (*i > 2)
			close(*i);
		k = red->herdoc_fd;
	}
	if (k == -1)
	{
		set_exit_status(1, 1337);
		return (-3);
	}
	return (k);
}
