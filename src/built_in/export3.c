/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:22:39 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:29:35 by aboukhmi         ###   ########.fr       */
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

static void	update_value(t_env *env, char *new)
{
	char	*tmp;

	tmp = ft_strdup(env->value);
	free(env->value);
	env->value = ft_strjoin(tmp, new);
	free(tmp);
}

static int	update_full(t_env *env, char *new)
{
	char	*tmp;

	if (env->is_print == 0)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (0);
		free(env->full);
		env->full = ft_strjoin(tmp, env->value);
		free(tmp);
		env->is_print = 1;
	}
	else
	{
		tmp = ft_strdup(env->full);
		if (!tmp)
			return (0);
		free(env->full);
		env->full = ft_strjoin(tmp, new);
		free(tmp);
	}
	return (1);
}

void	update(char *key, t_env **env, char *new)
{
	t_env	*copy;

	copy = *env;
	while (copy && copy->key)
	{
		if (!ft_strcmp(key, copy->key))
		{
			update_value(copy, new);
			if (!copy->value)
				return ;
			if (!update_full(copy, new))
				return ;
			break ;
		}
		copy = copy->next;
	}
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
