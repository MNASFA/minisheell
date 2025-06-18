/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:32:56 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 13:39:13 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	handle_builtin_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	(*exee)->saved_in = dup(STDIN_FILENO);
	(*exee)->saved_out = dup(STDOUT_FILENO);
	if ((*exee)->cd_in != STDIN_FILENO)
		dup2((*exee)->cd_in, STDIN_FILENO);
	if ((*exee)->cd_out != STDOUT_FILENO)
		dup2((*exee)->cd_out, STDOUT_FILENO);
	custom_execve(NULL, cmd, env, exee);
	dup2((*exee)->saved_in, STDIN_FILENO);
	dup2((*exee)->saved_out, STDOUT_FILENO);
	close((*exee)->saved_in);
	close((*exee)->saved_out);
	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
}

static void	handle_fork_error(t_exee **exee)
{
	perror("fork");
	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
	set_exit_status(1, 1337);
}

static void	handle_parent_process(t_exee **exee, pid_t pid)
{
	int	status;

	signal (SIGINT, SIG_IGN);
	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status), 0);
	if (WIFSIGNALED(status))
		set_exit_status(WTERMSIG(status) + 128, 0);
}

static void	handle_external_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process(exee, cmd, env);
	else if (pid > 0)
		handle_parent_process(exee, pid);
	else
		handle_fork_error(exee);
}

int	handle_single_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	(*exee)->cd_in = STDIN_FILENO;
	(*exee)->cd_out = STDOUT_FILENO;
	if (!cmd || !(*cmd))
		return (0);
	if ((*cmd) && !(*cmd)->cmd)
	{
		if ((*cmd)->redirections)
		{
			if (open_in_out(cmd) == -1)
				return (set_exit_status(1, 1337));
		}
		return (0);
	}
	setup_command_io(exee, cmd, 0);
	if ((*exee)->cd_in == -1 || (*exee)->cd_out == -1)
		return (set_exit_status(1, 1337));
	(*exee)->fd_in = (*exee)->cd_in;
	(*exee)->fd_out = (*exee)->cd_out;
	if (is_built_in((*cmd)->cmd))
		handle_builtin_command(exee, cmd, env);
	else
		handle_external_command(exee, cmd, env);
	return (0);
}
