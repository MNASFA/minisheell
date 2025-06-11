/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/11 14:39:20 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"
#include <string.h>

t_env	*find_in_env(char *key, t_env *env)
{
	while (env && ft_strcmp(env->key, key))
		env = env->next;
	return (env);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	export_env_var(char *key, char *value, t_env **env)
{
	char	*tmp;
	char	*str;
	char	**args;

	tmp = ft_strjoin(key, "=");
	str = ft_strjoin(tmp, value);
	free(tmp);
	args = malloc(3 * sizeof(char *));
	args[0] = ft_strdup("export");
	args[1] = str;
	args[2] = NULL;
	ft_export(args, env);
	free(args[0]);
	free(args[1]);
	free(args);
}

void	update_old_pwd(char *av, t_env **env)
{
	t_env	*str;
	char	*oo;

	str = find_in_env("PWD", *env);
	if (!str)
	{
		oo = satic_stock(av, env);
		export_env_var("OLDPWD", oo, env);
		free(oo);
		return ;
	}
	export_env_var("OLDPWD", str->value, env);
}

void	go_home(t_env **env)
{
	t_env	*home;

	home = find_in_env("HOME", *env);
	if (!home || chdir(home->value) == -1)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", 2);
		set_exit_status(1, 1337);
		return ;
	}
}

void	process_6(t_env **env)
{
	t_env	*str;

	str = find_in_env("OLDPWD", *env);
	if (chdir(str->value) == -1)
	{
		perror("bash: cd: ");
		write(2, str->value, ft_strlen(str->value));
		write (2, ":No such file or directory\n", 27);
		set_exit_status(1, 1337);
		return ;
	}
	pwd(*env);
}


static void	handle_cd_error(char *arg, char *oldpwd)
{
	write(2, "minishell: cd:", 15);
	write(2, arg, ft_strlen(arg));
	write (2, ":No such file or directory\n", 27);
	set_exit_status(1, 1337);
	free(oldpwd);
}

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

void	cd(char **arg, t_env **env)
{
	char	*oldpwd;

	(*env)->is_first = 0;
	if (arg[1] && arg[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		set_exit_status(1, 1337);
		return;
	}
	if (!arg[1])
	{
		go_home(env);
		return;
	}
	if (!ft_strcmp(arg[1], "-"))
		process_6(env);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		if (find_in_env("PWD", *env))
			oldpwd = ft_strdup(find_in_env("PWD", *env)->value);
	}
	if (chdir(arg[1]) == -1 && ft_strcmp(arg[1], "-"))
	{
		handle_cd_error(arg[1], oldpwd);
		return ;
	}
	update_pwd_vars(oldpwd, env);
	free(oldpwd);
}
