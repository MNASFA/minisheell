/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:19 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:44:13 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

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
