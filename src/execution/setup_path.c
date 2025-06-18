/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:03:21 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/17 21:44:24 by aboukhmi         ###   ########.fr       */
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
		return (freeee(env), NULL);
	dir = ft_split_exe(env[i] + 5, ':');
	freeee(env);
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

void	cle_env_fds(t_exee **exe)
{
	safe_close(&(*exe)->fd_in);
	safe_close(&(*exe)->fd_out);
}

char	*get_full_path_f(char *argv, t_env **env, t_exee **exe)
{
	if (!argv)
		return (NULL);
	if (argv && argv[0] == '\0')
		return (ft_strdup(""));
	if (ft_strchr(argv, '/'))
	{
		if (is_directory(argv))
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(argv, 2),
				ft_putstr_fd(": Is a directory\n", 2),
				cle_env_fds(exe), exit(set_exit_status(126, 1337)), NULL);
		if (access(argv, F_OK) != 0)
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(argv, 2),
				ft_putstr_fd(": No such file or directory\n", 2),
				cle_env_fds(exe), exit(set_exit_status(127, 1337)), NULL);
		if (access(argv, X_OK) != 0)
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(argv, 2),
				ft_putstr_fd(": Permission denied\n", 2),
				cle_env_fds(exe), exit(set_exit_status(126, 1337)), NULL);
		return (ft_strdup(argv));
	}
	if (access(argv, X_OK) == 0)
		return (ft_strdup(argv));
	return (get_full_path(argv, env));
}
