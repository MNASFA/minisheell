/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:03:21 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 19:19:49 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*create_full_path(char *dir_path, char *argv)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(dir_path, "/");
	if (!temp)
		return (NULL);
	temp2 = ft_strjoin(temp, argv);
	free(temp);
	return (temp2);
}

static char	*search_in_directories(char **dir, char *argv)
{
	char	*temp2;
	char	*path;
	int		i;
	int		existence;

	i = 0;
	while (dir[i])
	{
		temp2 = create_full_path(dir[i], argv);
		if (!temp2)
			return (freeee(dir), NULL);
		free(dir[i]);
		dir[i] = temp2;
		existence = access(dir[i], X_OK);
		if (existence == 0)
		{
			path = ft_strdup(dir[i]);
			freeee(dir);
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*get_full_path(char *argv, t_env **envi)
{
	char	**dir;
	char	**env;
	char	*result;
	int		i;

	if (!argv || !envi || !(*envi))
		return (NULL);
	env = env_list_to_array(*envi);
	if (!env)
		return (NULL);
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (free(env), NULL);
	dir = ft_split_exe(env[i] + 5, ':');
	free(env);
	if (!dir)
		return (NULL);
	result = search_in_directories(dir, argv);
	if (!result)
		freeee(dir);
	return (result);
}

int	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf))
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

char	*get_full_path_f(char *argv, t_env **env)
{
	if (!argv)
		return (NULL);
	if (argv && argv[0] == '\0')
		return (ft_strdup(""));
	if (strchr(argv, '/') && is_directory(argv))
		return (ft_putstr_fd("minishell: /: Is a directory\n", 2),
			cle_env_fds(env), exit(set_exit_status(126, 1337)), NULL);
	else if (strncmp(argv, "/", 1) == 0
		&& !is_directory(argv) && access(argv, X_OK) != 0)
	{
		return (ft_putstr_fd("minishell: /: no such file or directory\n", 2)
			, cle_env_fds(env), exit(set_exit_status(127, 1337)), NULL);
	}
	if (strchr(argv, '/'))
	{
		if (access(argv, X_OK) == 0)
			return (ft_strdup(argv));
		else
			return (ft_putstr_fd("minishell: /: no such file or directory\n", 2)
				, cle_env_fds(env), exit(set_exit_status(127, 1337)), NULL);
	}
	else if (access(argv, X_OK) == 0)
		return (ft_strdup(argv));
	else
		return (get_full_path(argv, env));
}
