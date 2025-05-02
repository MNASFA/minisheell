/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/01 19:07:10 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

char **parse_args(char *str)
{
	char **args;
	args = ft_split_exe(str, '/');
	return(args);
}
char    *get_path_2(char *str)
{
	int n;
	char *new_path;

	n = ft_strlen(str);
	while (str[n -1] && str[n -1] != '/')
		n--;
	new_path = (char *)malloc(n + 1);
	strncpy(new_path, str, n);
	new_path[n] = '\0';
	free(str);
	return (new_path);
	
}

char *pwd1()
{
	char	*path;

	path = (char *)malloc(PATH_MAX);
	getcwd(path, PATH_MAX);
	return (path);
}

void cd(char *av)
{
	char *path;
	char **arg;

	arg = parse_args(av);
	int i;
	i = 0;

	if (!av)
	{
		chdir(getenv("HOME"));
	}
	while (arg[i])
	{
		if (!strcmp(arg[i], "."))
		{
			i++;
			continue;
		}
		else if (!strcmp(arg[i], "~"))
			chdir(getenv("HOME"));
		else if (!ft_strcmp(arg[i], ".."))
		{
			path = pwd1();
			chdir(getenv("HOME"));
			path = get_path_2(path);
			chdir(path);
			pwd1();
			free(path);
		}
		else
			chdir(arg[i]);
		i++;
	}

}