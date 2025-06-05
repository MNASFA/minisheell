/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:11:10 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/26 16:31:37 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*create_env_var(char *env_start)
{
	t_env	*new_var;
	char	*equal;

	equal = ft_strchr(env_start, '=');
	if (!equal)
		return (NULL);
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strndup(env_start, equal - env_start);
	new_var->value = ft_strdup(equal + 1);
	new_var->full = ft_strdup(env_start);
	new_var->next = NULL;
	return (new_var);
}

static char	**create_env_with_pwd(char *str)
{
	char	**args;
	char	*temp;

	args = malloc(5 * sizeof(char *));
	if (!args)
		return (free(str), NULL);
	temp = ft_strjoin("PWD=", str);
	args[0] = ft_strdup(temp);
	free(temp);
	free(str);
	args[1] = ft_strdup("SHLVL=1");
	args[2] = ft_strdup("_=/usr/bin/env");
	args[3] = ft_strdup("PATH=/home/aboukhmi/bin:/usr/local/sbin:/usr/local/bin:\
		/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
	args[4] = NULL;
	return (args);
}

static char	**create_env_without_pwd(void)
{
	char	**args;

	args = malloc(4 * sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup("SHLVL=1");
	args[1] = ft_strdup("_=/usr/bin/env");
	args[2] = ft_strdup("PATH=/home/aboukhmi/bin:/usr/local/sbin:/usr/local/bin:\
		/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin");
	args[3] = NULL;
	return (args);
}

char	**env_no_env(void)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (str)
		return (create_env_with_pwd(str));
	else
		return (create_env_without_pwd());
}

char	*get_env_value(t_env *env, char *key)
{
	if (!env || !key || *key == '\0')
		return ("");
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}
