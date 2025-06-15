/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:16:45 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 19:49:25 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	setup_redirections(t_exee **exee)
{
	if ((*exee)->cd_in != STDIN_FILENO)
	{
		if (dup2((*exee)->cd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(set_exit_status(1, 1337));
		}
	}
	if ((*exee)->cd_out != STDOUT_FILENO)
	{
		if (dup2((*exee)->cd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(set_exit_status(1, 1337));
		}
	}
}

static void	cleanup_pipe_fds(t_exee **exee)
{
	if ((*exee)->cmd_count > 1)
	{
		close_all_pipes(*exee);
		if ((*exee)->cd_in > 2)
			close((*exee)->cd_in);
		if ((*exee)->cd_out > 2)
			close((*exee)->cd_out);
	}
}

static void	handle_command_not_found(char *str, t_exec **cmd, t_exee **exee)
{
	if (str && str[0] == '\0')
	{
		write(2, "Command '' not found\n", 22);
		free(str);
	}
	else
	{
		write (2, (*cmd)->cmd, ft_strlen((*cmd)->cmd));
		write(2, ": Command not found\n", 20);
	}
	cleanup_exe(*exee);
	set_exit_status(127, 1337);
	exit(127);
}

static void	execute_external_command(char **str, t_exec **cmd, t_env **env,
			t_exee **exee)
{
	*str = get_full_path_f((*cmd)->cmd, env);
	if (!*str || (*str && (*str)[0] == '\0'))
		handle_command_not_found(*str, cmd, exee);
	custom_execve(*str, cmd, env, exee);
}

void	execute_child_process(t_exee **exee, t_exec **cmd, t_env **env)
{
	char	*str;

	str = NULL;
	if (!cmd || !(*cmd))
		exit(set_exit_status(1, 1337));
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!(*cmd)->cmd)
		exit(set_exit_status(0, 1337));
	setup_redirections(exee);
	(*cmd)->outfd = (*exee)->cd_out;
	(*cmd)->infd = (*exee)->cd_in;
	cleanup_pipe_fds(exee);
	if (is_built_in((*cmd)->args[0]))
		custom_execve(NULL, cmd, env, exee);
	else
		execute_external_command(&str, cmd, env, exee);
	free(str);
}
