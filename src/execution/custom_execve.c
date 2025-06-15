/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_execve.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:09:48 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 20:59:52 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	execute_builtin(t_exec **cmd, t_env **env, t_exee **exe)
{
	if (!ft_strcmp((*cmd)->args[0], "echo"))
		ft_echo((*cmd)->args);
	else if (!ft_strcmp((*cmd)->args[0], "cd"))
		cd((*cmd)->args, env);
	else if (!ft_strcmp((*cmd)->args[0], "pwd"))
		pwd(*env);
	else if (!ft_strcmp((*cmd)->args[0], "exit"))
		ft_exit(cmd, 0, env, exe);
	else if (!ft_strcmp((*cmd)->args[0], "export"))
		ft_export((*cmd)->args, env);
	else if (!ft_strcmp((*cmd)->args[0], "env"))
		print_env((*cmd)->args, *env);
	else if (!ft_strcmp((*cmd)->args[0], "unset"))
		ft_unset(env, (*cmd)->args);
	else
		return (0);
	return (1);
}

static int	execute_external(char *str, t_exec **cmd, t_env **env, t_exee **exe)
{
	char	**env_array;

	env_array = env_list_to_array(*env);
	if (!env_array)
		exit (set_exit_status(1, 1337));
	if (execve(str, (*cmd)->args, env_array) == -1)
	{
		perror(str);
		free(env_array);
		cleanup_exe(*exe);
		exit(set_exit_status(127, 1337));
	}
	free(env_array);
	return (0);
}

int	custom_execve(char *str, t_exec **cmd, t_env **env, t_exee **exe)
{
	if ((cmd && !(*cmd)->args) || !(*cmd)->args || !(*cmd)->args[0])
		return (-1);
	if (execute_builtin(cmd, env, exe))
	{
		safe_close(&(*exe)->infile);
		safe_close(&(*exe)->outfile);
		if ((*exe)->cmd_count != 1)
			exit(set_exit_status(0, -1));
		return (0);
	}
	return (execute_external(str, cmd, env, exe));
}
