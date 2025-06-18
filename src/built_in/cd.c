/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 12:22:03 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static void	handle_newpwd_failure(t_env **env, char *arg)
{
	char	*prev;
	char	*newpwd;
	char	*tmp;

	if (find_in_env("PWD", *env))
		prev = find_in_env("PWD", *env)->value;
	else
		prev = ft_strdup("..");
	newpwd = ft_strjoin(prev, "/..");
	// free (prev);
	tmp = newpwd;
	newpwd = ft_strjoin(tmp, arg);
	free(tmp);
	perror("cd: error retrieving current directory: getcwd: cannot access"
		" parent directories: No such file or directory");
	export_env_var("PWD", newpwd, env);
	free(newpwd);
}

static void	update_pwd_vars(char *oldpwd, t_env **env)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		handle_newpwd_failure(env, NULL);
	else
	{
		export_env_var("OLDPWD", oldpwd, env);
		export_env_var("PWD", newpwd, env);
		free(newpwd);
	}
}

static char	*get_oldpwd(t_env *env)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		if (find_in_env("PWD", env))
			oldpwd = ft_strdup(find_in_env("PWD", env)->value);
	}
	return (oldpwd);
}

static int	handle_cd_args(char **arg, t_env **env)
{
	if (arg[1] && arg[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		set_exit_status(1, 1337);
		return (1);
	}
	if (!arg[1])
	{
		go_home(env);
		return (1);
	}
	if (!ft_strcmp(arg[1], "-"))
	{
		process_6(env);
		return (1);
	}
	return (0);
}

void	cd(char **arg, t_env **env)
{
	char	*oldpwd;

	if (handle_cd_args(arg, env))
		return ;
	oldpwd = get_oldpwd(*env);
	if (chdir(arg[1]) == -1 && ft_strcmp(arg[1], "-"))
	{
		handle_cd_error(arg[1], oldpwd);
		return ;
	}
	update_pwd_vars(oldpwd, env);
	free(oldpwd);
}
