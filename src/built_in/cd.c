/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/17 23:05:55 by aboukhmi         ###   ########.fr       */
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
		return ;
	}
	export_env_var("OLDPWD", str->value, env);
}

void	go_home(t_env **env)
{
	t_env	*home;

	home = find_in_env("HOME", *env);
	if (chdir(home->value) == -1)
	{
		perror("bash: cd: HOME not set");
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
		printf("%s: No such file or directory\n", str->value);
		set_exit_status(1, 1337);
		return ;
	}
	pwd(*env);
}

char	*normal_component(char *str)
{
	char	*s;
	char	*ret;

	if (str[0] == '/')
		return (str);
	s = getcwd(0, 0);
	ret = ft_strjoin(s, "/");
	free (s);
	s = ft_strjoin(ret, str);
	return (s);
}

// void process_path_component(char *av, t_env **env)
// {
//     char *str;

//     str = NULL;
//     if (strcmp(av, "-") == 0)
//         process_6(env);
//     else if (!strcmp(av, "."))
//         return;
//     else if(chdir(av) == -1)
//     {
//         perror("bash: cd: ");
//         printf("%s: No such file or directory\n", str);
//         return;
//     }
// }

static void	handle_cd_error(char *arg, char *oldpwd)
{
	write(2, "minishell: cd:", 15);
	printf("%s:No such file or directory\n", arg);
	set_exit_status(1, 1337);
	free(oldpwd);
}

static void	handle_newpwd_failure(t_env **env, char *arg)
{
	char	*prev;
	char	*newpwd;
	char	*tmp;

	prev = find_in_env("PWD", *env)->value;
	newpwd = ft_strjoin(prev, "/");
	tmp = newpwd;
	newpwd = ft_strjoin(tmp, arg);
	free(tmp);
	perror("cd: error retrieving current directory: getcwd: cannot access"
		" parent directories: No such file or directory");
	set_exit_status(0, 1337);
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

void	cd(char *arg, t_env **env)
{
	char	*oldpwd;

	(*env)->is_first = 0;
	if (!ft_strcmp(arg, "-"))
		process_6(env);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup(find_in_env("PWD", *env)->value);
	if (chdir(arg) == -1 && ft_strcmp(arg, "-"))
	{
		handle_cd_error(arg, oldpwd);
		return ;
	}
	update_pwd_vars(oldpwd, env);
	free(oldpwd);
}
