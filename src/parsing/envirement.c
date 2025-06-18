/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:37:51 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/17 19:45:29 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	add_env_var(t_env **head, t_env **current, char *env_str)
{
	t_env	*new_var;

	new_var = create_env_var(env_str);
	if (!new_var)
		return (0);
	new_var->is_first = 1;
	new_var->is_print = 1;
	if (!*head)
		*head = new_var;
	else
		(*current)->next = new_var;
	*current = new_var;
	return (1);
}

t_env	*init_env(char **envp, int i, int generated_env)
{
	t_env	*head;
	t_env	*current;

	head = NULL;
	current = NULL;
	if (envp && !envp[0])
	{
		generated_env = 1;
		envp = env_no_env();
	}
	while (envp && envp[i])
	{
		if (!add_env_var(&head, &current, envp[i]))
		{
			free_env_list(head);
			if (generated_env == 1)
				freeee(envp);
			return (NULL);
		}
		i++;
	}
	if (generated_env == 1)
		freeee(envp);
	return (head);
}

static int	calc_varname_len(char *str, int i)
{
	int	len;

	len = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		len++;
		i++;
	}
	return (len);
}

static int	copy_var_name(char *str, int i, int var_len, char **var_name)
{
	int	j;

	j = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'
			|| str[i] == '?') && j < var_len)
	{
		(*var_name)[j++] = str[i++];
		if (str[i - 1] == '?')
			break ;
	}
	(*var_name)[j] = '\0';
	return (i);
}

int	extract_var_name(char *str, int i, char **var_name)
{
	int	var_len;
	int	start_i;

	start_i = i;
	if (str[i] == '$')
		i++;
	if (!str[i] || !(ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?'))
	{
		*var_name = NULL;
		return (start_i + 1);
	}
	var_len = calc_varname_len(str, i);
	if (var_len == 0)
	{
		*var_name = NULL;
		return (i);
	}
	*var_name = malloc(var_len + 1);
	if (!*var_name)
		return (i);
	return (copy_var_name(str, i, var_len, var_name));
}
