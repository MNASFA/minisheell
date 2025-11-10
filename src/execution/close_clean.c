/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:41:49 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:21:12 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_all_pipes(t_exee *exee)
{
	int	i;

	if (!exee || !exee->pipes || exee->cmd_count <= 1)
		return ;
	i = 0;
	while (i < exee->cmd_count - 1)
	{
		if (exee->pipes[i])
		{
			close(exee->pipes[i][0]);
			close(exee->pipes[i][1]);
		}
		i++;
	}
}

void	cleanup_exe(t_exee *exe)
{
	int	i;

	if (!exe)
		return ;
	if (exe->pipes && exe->cmd_count > 1)
	{
		i = 0;
		while (i < exe->cmd_count - 1)
		{
			if (exe->pipes[i])
				free(exe->pipes[i]);
			i++;
		}
		free(exe->pipes);
	}
	if (exe->pids)
		free(exe->pids);
	if (exe->infile > 2)
		close(exe->infile);
	if (exe->outfile > 2)
		close(exe->outfile);
	free(exe);
}

void	closeallfiles(t_exec **commands)
{
	t_exec	*command;

	if (!commands || !(*commands))
		return ;
	command = *commands;
	while (command)
	{
		if (command->outfd > 2)
		{
			close(command->outfd);
			command->outfd = -2;
		}
		if (command->infd > 2)
		{
			close(command->infd);
			command->infd = -2;
		}
		command = command->next;
	}
}

void	safe_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -2;
	}
}

int	is_built_in(char *str)
{
	if (!str)
		return (0);
	if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "exit")
		|| !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "unset"))
		return (1);
	return (0);
}
