/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:15:01 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 20:14:30 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_commands(t_exee **exee, t_exec **cmds, t_env **env)
{
	if (!exee || !cmds)
		return ;
	if ((*exee)->cmd_count == 1)
		handle_single_command(exee, cmds, env);
	else
		handle_pipeline(exee, cmds, env);
	if ((*exee)->infile > 2)
		close((*exee)->infile);
	if ((*exee)->outfile > 2)
		close((*exee)->outfile);
}

static void	cleanup_env_fds(t_exee **exe)
{
	if ((*exe)->fd_in > 2)
	{
		close((*exe)->fd_in);
		(*exe)->fd_in = -1;
	}
	if ((*exe)->fd_out > 2)
	{
		close((*exe)->fd_out);
		(*exe)->fd_out = -1;
	}
}

static void	handle_pipeline_cleanup(t_exee **exe, t_exec **commands)
{
	wait_for_all_children(*exe);
	cleanup_env_fds(exe);
	cleanup_exe(*exe);
	closeallfiles(commands);
}

void	execution(t_exec **commands, t_env **envi)
{
	t_exee	*exe;

	if (!commands || !(*commands))
		return ;
	exe = init_execution(commands);
	if (!exe)
	{
		set_exit_status(1, 1337);
		return ;
	}
	execute_commands(&exe, commands, envi);
	if (exe->cmd_count == 1 && (*commands)->cmd
		&& is_built_in((*commands)->cmd))
		cleanup_exe(exe);
	else
		handle_pipeline_cleanup(&exe, commands);
}
