/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:45:11 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 13:24:03 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	ft_lstsize(t_env *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
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
