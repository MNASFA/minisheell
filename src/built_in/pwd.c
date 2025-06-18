/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:27 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 12:02:03 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static char	*handle_getcwd_fail(char *cmd, t_env **env)
{
	char	*path;
	char	*temp;

	if (!find_in_env("PWD", *env))
		return (NULL);
	path = ft_strdup(find_in_env("PWD", *env)->value);
// 	if (access(path, F_OK) == -1 && (*env)->is_first == 1)
// 	{
// 		perror("pwd: error retrieving current directory: getcwd: \
// cannot access parent directories: No such file or directory");
// 		set_exit_status(1, 1337);
// 		free(path);
// 		return (NULL);
// 	}
	if (cmd && (!ft_strcmp(cmd, "..") || !ft_strcmp(cmd, ".")))
	{
		temp = path;
		path = ft_strjoin(path, cmd);
		free(temp);
	}
	return (path);
}

static void	cleanup_static_path(void)
{
	static char	*path;

	if (path)
	{
		free(path);
		path = NULL;
	}
}

char	*satic_stock(char *cmd, t_env **env)
{
	char		*str;
	static char	*path;

	if (cmd && !ft_strcmp(cmd, "cleanup"))
		return (cleanup_static_path(), NULL);
	if (path)
	{
		free(path);
		path = NULL;
	}
	str = getcwd(0, 0);
	if (!str)
		path = handle_getcwd_fail(cmd, env);
	else
	{
		path = ft_strdup(str);
		free(str);
	}
	return (path);
}

void	cleanup_pwd(void)
{
	satic_stock("cleanup", NULL);
}

void	pwd(t_env *env)
{
	char	*str;

	str = satic_stock(0, &env);
	if (str)
		printf("%s\n", str);
	set_exit_status(0, 1337);
}
