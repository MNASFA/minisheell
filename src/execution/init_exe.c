/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:49:56 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/17 20:01:28 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	ft_lstsize_commands(t_exec **lst)
{
	int		i;
	t_exec	*current;

	if (!lst || !(*lst))
		return (0);
	i = 0;
	current = *lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

static int	init_pipes_and_pids(t_exee *exe)
{
	int	i;

	exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
	if (!exe->pipes)
		return (-1);
	i = 0;
	while (i < exe->cmd_count - 1)
	{
		exe->pipes[i] = NULL;
		i++;
	}
	exe->pids = malloc(sizeof(int) * exe->cmd_count);
	if (!exe->pids)
	{
		free(exe->pipes);
		return (-1);
	}
	i = 0;
	while (i < exe->cmd_count)
	{
		exe->pids[i] = 0;
		i++;
	}
	return (0);
}

static t_exee	*cleanup_and_return_null(t_exee *exe)
{
	if (exe->infile != -1)
		close(exe->infile);
	if (exe->outfile != -1)
		close(exe->outfile);
	free(exe);
	return (NULL);
}

static t_exee	*init_basic_fields(t_exec **commands)
{
	t_exee	*exe;

	exe = malloc(sizeof(t_exee));
	if (!exe)
		return (NULL);
	exe->cmd_count = ft_lstsize_commands(commands);
	exe->infile = dup(STDIN_FILENO);
	exe->outfile = dup(STDOUT_FILENO);
	exe->cd_in = -2;
	exe->cd_out = -2;
	exe->saved_in = -1;
	exe->saved_out = -2;
	exe->fd_in = -2;
	exe->fd_out = -2;
	exe->pipes = NULL;
	exe->pids = NULL;
	if (exe->infile == -1 || exe->outfile == -1)
		return (cleanup_and_return_null(exe));
	return (exe);
}

t_exee	*init_execution(t_exec **commands)
{
	t_exee	*exe;

	exe = init_basic_fields(commands);
	if (!exe)
		return (NULL);
	if (exe->cmd_count > 1)
	{
		if (init_pipes_and_pids(exe) == -1)
			return (cleanup_and_return_null(exe));
	}
	return (exe);
}
