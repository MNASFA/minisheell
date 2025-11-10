/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:22:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/17 21:09:47 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static t_env	*ft_lstlast_ex(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_ex(t_env **lst, t_env *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast_ex(*lst)->next = new;
	else
		*lst = new;
}

t_env	*free_all_env(t_env *to_free)
{
	t_env	*next_node;

	if (!to_free)
		return (NULL);
	next_node = to_free->next;
	free(to_free->key);
	free(to_free->value);
	free(to_free->full);
	free(to_free);
	return (next_node);
}

void	delete_if_exist(t_env **env, char *to_delete)
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

int	is_there_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			if (i > 0 && str[i - 1] && str[i - 1] == '+')
				return (2);
			return (1);
		}
		i++;
	}
	return (0);
}
