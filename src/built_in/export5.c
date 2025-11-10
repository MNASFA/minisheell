/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:59:35 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 16:02:37 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

void	print_sorted(t_env **env)
{
	int		count;
	t_env	*temp;
	t_env	**env_array;
	int		*printed;

	temp = *env;
	count = env_count_hh(temp);
	if (count == 0)
		return ;
	env_array = env_t_array(*env, count);
	if (!env_array)
		return ;
	printed = printed_tracker(env_array, count);
	if (!printed)
		return ;
	printtt(env_array, printed, count);
	free(printed);
	free(env_array);
}

int	is_in_env(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (1);
		env = env->next;
	}
	return (0);
}
