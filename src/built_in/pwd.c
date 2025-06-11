/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:27 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/10 13:37:06 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"
#include <limits.h>

static t_env	*find_in_env(char *key, t_env *env)
{
	while (env && ft_strcmp(env->key, key))
		env = env->next;
	return (env);
}

char	*satic_stock(char *cmd, t_env **env)
{
	char		*str;
	static char	*path;

	str = getcwd(0, 0);
	if (!str)
	{
		if(find_in_env("PWD", *env))
			path = find_in_env("PWD", *env)->value;
		if (access(path, F_OK) == -1 && (*env)->is_first == 1)
		{
			perror("pwd: error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory");
			set_exit_status(1, 1337);
			return (NULL);
		}
		if (!ft_strcmp(cmd, "..") || !ft_strcmp(cmd, "."))
			path = ft_strjoin(path, cmd);
	}
	else
		path = ft_strdup(str);
	return (free(str), path);
}

void	pwd(t_env *env)
{
	char	*str;

	str = satic_stock(0, &env);
	if (str)
		printf("%s\n", str);
	free (str);
}
