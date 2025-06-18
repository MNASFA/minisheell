/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:04:28 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 18:07:49 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	setup_first_command_io(t_exee **exee, t_exec **cmd)
{
	if (!cmd || !(*cmd) || !(*exee))
		return ;
	if ((*cmd)->redirections)
	{
		if (open_in_out(cmd) == -1)
		{
			(*exee)->cd_in = -1;
			(*exee)->cd_out = -1;
			return ;
		}
		if ((*cmd)->infd != -2)
			(*exee)->cd_in = (*cmd)->infd;
		if ((*cmd)->outfd != -2)
			(*exee)->cd_out = (*cmd)->outfd;
	}
	if ((*cmd)->infd == -2)
		(*exee)->cd_in = (*exee)->infile;
	if ((*exee)->cmd_count > 1 && (*cmd)->outfd == -2)
		(*exee)->cd_out = (*exee)->pipes[0][1];
	else if ((*cmd)->outfd == -2)
		(*exee)->cd_out = (*exee)->outfile;
}

void	setup_middle_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (!cmd || !(*cmd) || !(*exee))
		return ;
	if ((*cmd)->redirections)
	{
		if (open_in_out(cmd) == -1)
		{
			(*exee)->cd_in = -1;
			(*exee)->cd_out = -1;
			return ;
		}
		if (is_there_any_in((*cmd)->redirections))
		{
			safe_close(&(*exee)->pipes[cmd_index - 1][0]);
			(*exee)->cd_in = (*cmd)->infd;
		}
		if (is_there_any_out((*cmd)->redirections))
		{
			safe_close(&(*exee)->pipes[cmd_index][1]);
			(*exee)->cd_out = (*cmd)->outfd;
		}
	}
	if (!is_there_any_in((*cmd)->redirections))
		(*exee)->cd_in = (*exee)->pipes[cmd_index - 1][0];
	if (!is_there_any_out((*cmd)->redirections))
		(*exee)->cd_out = (*exee)->pipes[cmd_index][1];
}

void	setup_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (cmd_index == 0)
		setup_first_command_io(exee, cmd);
	else if (cmd_index == (*exee)->cmd_count - 1)
		setup_last_command_io(exee, cmd, cmd_index);
	else
		setup_middle_command_io(exee, cmd, cmd_index);
}

void	cle_env_fds(t_exee **exe)
{
	safe_close(&(*exe)->fd_in);
	safe_close(&(*exe)->fd_out);
}
