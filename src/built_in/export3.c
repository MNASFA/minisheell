/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:22:39 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/15 16:05:01 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

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

char	*join_with_equal(char *str1, char *str2)
{
	char	*temp;
	char	*result;

	if (!str1 || !str2)
		return (NULL);
	temp = ft_strjoin(str1, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, str2);
	free(temp);
	return (result);
}

static void	create_new_node(char *key, char *new, t_env **env)
{
	t_env	*tmp;

	tmp = (t_env *)malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(new);
	tmp->full = join_with_equal(key, new);
	tmp->next = NULL;
	tmp->is_print = 1;
	ft_lstadd_back_ex(env, tmp);
}

void	update(char *key, t_env **env, char *new)
{
	t_env	*copy;
	int		found;

	copy = *env;
	found = 0;
	while (copy && copy->key)
	{
		if (!ft_strcmp(key, copy->key))
		{
			found = 1;
			update_value(copy, new);
			if (!copy->value || !update_full(copy, new))
				return ;
			break ;
		}
		copy = copy->next;
	}
	if (!found)
		create_new_node(key, new, env);
}
