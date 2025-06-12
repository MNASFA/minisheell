/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:37:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:41:50 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	handle_io_error(t_exee **exee, int i)
{
	(*exee)->pids[i] = 0;
	set_exit_status(1, 1337);
}

static void	cleanup_command_fds(t_exec *cmd)
{
	if (cmd->infd > 2)
	{
		close(cmd->infd);
		cmd->infd = -1;
	}
	if (cmd->outfd > 2)
	{
		close(cmd->outfd);
		cmd->outfd = -1;
	}
}

static void	process_single_command(t_exee **exee, t_exec **cmd, t_env **env,
			int i)
{
	t_exec	*temp_cmd;

	(*exee)->cd_in = STDIN_FILENO;
	(*exee)->cd_out = STDOUT_FILENO;
	temp_cmd = *cmd;
	setup_command_io(exee, &temp_cmd, i);
	if ((*exee)->cd_in == -1 || (*exee)->cd_out == -1)
		handle_io_error(exee, i);
	else
	{
		if ((*cmd)->cmd)
		{
			(*exee)->pids[i] = fork();
			if ((*exee)->pids[i] == 0)
			{
				signal(SIGINT, SIG_DFL);
				execute_child_process(exee, &temp_cmd, env);
			}
		}
		else if (!(*cmd)->cmd)
			(*exee)->pids[i] = 0;
	}
	cleanup_command_fds(*cmd);
}

void	handle_pipeline(t_exee **exee, t_exec **cmds, t_env **env)
{
	t_exec	*cmd;
	int		i;

	if (!cmds || !(*cmds))
		return ;
	setup_pipes(*exee);
	cmd = *cmds;
	i = 0;
	while (i < (*exee)->cmd_count && cmd)
	{
		process_single_command(exee, &cmd, env, i);
		cmd = cmd->next;
		i++;
	}
	close_all_pipes((*exee));
}
