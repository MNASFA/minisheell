/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:22:24 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:28:27 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

int	find_next_min(t_env **env, int *printed, int size)
{
	int	min_idx;
	int	j;

	min_idx = -1;
	j = 0;
	while (j < size)
	{
		if (!printed[j])
		{
			if (min_idx == -1
				|| ft_strcmp(env[j]->full, env[min_idx]->full) < 0)
				min_idx = j;
		}
		j++;
	}
	return (min_idx);
}

int	env_count_hh(t_env *tmp)
{
	int	count;

	count = 0;
	if (!tmp)
		return (0);
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

t_env	**env_t_array(t_env *env, int count)
{
	t_env	**env_array;
	int		i;

	i = 0;
	env_array = malloc(count * sizeof(t_env *));
	if (!env_array)
		return (NULL);
	while (i < count)
	{
		env_array[i] = env;
		env = env->next;
		i++;
	}
	return (env_array);
}

int	*printed_tracker(t_env **env_array, int count)
{
	int	*printed;
	int	i;

	i = 0;
	printed = malloc(count * sizeof(int));
	if (!printed)
	{
		free(env_array);
		return (NULL);
	}
	while (i < count)
	{
		printed[i] = 0;
		i++;
	}
	return (printed);
}

void	printtt(t_env **env_array, int *printed, int count)
{
	int	i;
	int	min_idx;

	i = 0;
	while (i < count)
	{
		min_idx = find_next_min(env_array, printed, count);
		if (min_idx != -1 && min_idx < count)
		{
			printf("declare -x %s", env_array[min_idx]->key);
			if (env_array[min_idx]->value)
				printf("=\"%s\"\n", env_array[min_idx]->value);
			else if (!env_array[min_idx]->value
				&& env_array[min_idx]->is_print == 1)
			{
				printf("=\"\"\n");
			}
			else
				printf("\n");
			printed[min_idx] = 1;
		}
		i++;
	}
}
