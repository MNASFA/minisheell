/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:31 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/02 11:54:11 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	quotes_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && *in_double == 0)
		*in_single = !(*in_single);
	else if (c == '"' && *in_single == 0)
		*in_double = !(*in_double);
}

int	count_dollars(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i] == '$')
	{
		count++;
		i++;
	}
	return (count);
}

size_t	expanded_length(char *str, t_env *env)
{
	size_t	lenght;
	int		i;
	char	*var_name;

	i = 0;
	lenght = 0;
	var_name = NULL;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i++;
		else if (str[i] == '$' && str[i + 1])
		{
			i = extract_var_name(str, i, &var_name);
			lenght += ft_strlen(get_env_value(env, var_name));
			free (var_name);
		}
		else
		{
			lenght++;
			i++;
		}
	}
	return (lenght);
}

void	handle_variable_expansion(t_expand_vars *vars, char *result)
{
	int		dollar_count;
	char	*var_name;
	char	*value;

	var_name = NULL;
	dollar_count = count_dollars(vars->str, vars->i);
	if (dollar_count % 2 == 1)
	{
		vars->i += dollar_count - 1;
		if (vars->str[vars->i] == '$' && vars->str[vars->i + 1])
		{
			vars->i = extract_var_name(vars->str, vars->i, &var_name);
			value = get_env_value(vars->env, var_name);
			ft_strcpy(&result[vars->j], value);
			vars->j += ft_strlen(value);
		}
		else
		{
			result[vars->j++] = '$';
			vars->i++;
		}
	}
	else
		vars->i += dollar_count;
}

char	*expand_variables(char *str, t_env *env, int init_i, int init_j)
{
	t_expand_vars	vars;
	char			*result;

	init_expand_vars(&vars, str, env, init_i, init_j);
	result = malloc(expanded_length(str, env) + 1);
	if (!result)
		return (NULL);
	while (str[vars.i])
	{
		quotes_state(str[vars.i], &vars.in_single, &vars.in_double);
		if ((str[vars.i] == '\'' && !vars.in_double)
			|| (str[vars.i] == '\"' && !vars.in_single))
			vars.i++;
		if (str[vars.i] == '$' && !vars.in_single)
			handle_variable_expansion(&vars, result);
		else
			result[vars.j++] = str[vars.i++];
	}
	result[vars.j] = '\0';
	return (result);
}
