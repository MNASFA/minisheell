/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:02:12 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/17 22:09:49 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"



void	remove_from_env(t_env **env, char *to_delete)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !to_delete)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, to_delete) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			current = free_all_env(current);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void	ft_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	if (!args || !env)
		return ;
	while (args[i])
	{
		remove_from_env(env, args[i]);
		i++;
	}
	return ;
}
