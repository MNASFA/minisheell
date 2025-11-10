/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanded_length.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:00:02 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/17 20:27:38 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	handle_valid_var(char *var_name, t_env *env)
{
	char	*value;
	char	*exit_str;
	size_t	len;

	len = 0;
	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_str = ft_itoa(set_exit_status(1337, -1));
		if (exit_str)
		{
			len = ft_strlen(exit_str);
			free(exit_str);
		}
	}
	else
	{
		value = get_env_value(env, var_name);
		if (value)
			len = ft_strlen(value);
	}
	return (len);
}

static size_t	handle_dollar(char **str, int *i, t_env *env)
{
	int		start;
	char	*var_name;
	size_t	len;

	if ((*str)[*i + 1] == '$')
		return (*i += 2, 2);
	if (ft_isdigit((*str)[*i + 1]))
		return (*i += 2, 0);
	start = *i;
	var_name = NULL;
	*i = extract_var_name(*str, *i, &var_name);
	if (var_name && *var_name)
	{
		len = handle_valid_var(var_name, env);
		free(var_name);
		return (len);
	}
	free(var_name);
	*i = start + 1;
	return (1);
}

size_t	expanded_length(char *str, t_env *env, t_expand_vars *vars)
{
	size_t	length;
	int		i;

	if (!str)
		return (0);
	i = 0;
	length = 0;
	while (str[i])
	{
		quotes_state(str[i], &vars->in_single, &vars->in_double);
		if (str[i] == '$' && str[i + 1] && !vars->in_single)
			length += handle_dollar(&str, &i, env);
		else
		{
			length++;
			i++;
		}
	}
	return (length);
}

size_t	expanded_length_herdoc(char *str, t_env *env)
{
	size_t	length;
	int		i;

	if (!str)
		return (0);
	i = 0;
	length = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			length += handle_dollar(&str, &i, env);
		else
		{
			length++;
			i++;
		}
	}
	return (length);
}
