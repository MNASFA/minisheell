/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:45:11 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 21:02:15 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	ft_lstsize(t_env *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

char	**env_list_to_array(t_env *env)
{
	char	**enve;
	int		i;

	i = 0;
	enve = NULL;
	enve = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	if (!enve)
		return (NULL);
	while (env)
	{
		enve[i] = env->full;
		i++;
		env = env->next;
	}
	enve[i] = NULL;
	return (enve);
}

int	path_exists_in_env(t_env **env)
{
	char	**env_array;
	int		i;
	int		found;

	if (!env || !(*env))
		return (0);
	env_array = env_list_to_array(*env);
	if (!env_array)
		return (0);
	found = 0;
	i = 0;
	while (env_array[i])
	{
		if (ft_strncmp(env_array[i], "PATH=", 5) == 0)
		{
			found = 1;
			break ;
		}
		i++;
	}
	free(env_array);
	return (found);
}
