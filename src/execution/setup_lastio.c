/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_lastio.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:58:33 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 18:07:39 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_there_any_in(t_redir *red)
{
	while (red)
	{
		if (red->type == REDIR_IN || red->type == HEREDOC)
			return (1);
		red = red->next;
	}
	return (0);
}

int	is_there_any_out(t_redir *red)
{
	while (red)
	{
		if (red->type == REDIR_OUT || red->type == APPEND)
			return (1);
		red = red->next;
	}
	return (0);
}

static void	handle_redirections(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (open_in_out(cmd) == -1)
	{
		(*exee)->cd_in = -1;
		(*exee)->cd_out = -1;
		return ;
	}
	if (is_there_any_in((*cmd)->redirections))
	{
		close((*exee)->pipes[cmd_index - 1][0]);
		(*exee)->cd_in = (*cmd)->infd;
	}
	if (is_there_any_out((*cmd)->redirections))
	{
		(*exee)->cd_out = (*cmd)->outfd;
		if ((*exee)->cd_out == -2)
			(*exee)->cd_out = 1;
	}
}

static void	setup_default_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (!is_there_any_in((*cmd)->redirections))
		(*exee)->cd_in = (*exee)->pipes[cmd_index - 1][0];
	if (!is_there_any_out((*cmd)->redirections))
		(*exee)->cd_out = (*exee)->outfile;
}

void	setup_last_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (!cmd || !(*cmd) || !(*exee))
		return ;
	if ((*cmd)->redirections)
		handle_redirections(exee, cmd, cmd_index);
	setup_default_io(exee, cmd, cmd_index);
}
