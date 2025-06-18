/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:31 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 11:18:37 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_invalid_var(t_expand_vars *vars, char *result, int start)
{
	result[vars->j++] = '$';
	if (vars->str[start + 1])
	{
		result[vars->j++] = vars->str[start + 1];
		vars->i = start + 2;
	}
	else
		vars->i = start + 1;
}

static void	expand_and_copy_value(t_expand_vars *vars,
	char *result, char *var_name)
{
	char	*value;
	char	*exit_status_str;
	int		start;

	start = vars->i;
	vars->i = extract_var_name(vars->str, vars->i, &var_name);
	if (!var_name || !*var_name)
		return (handle_invalid_var(vars, result, start));
	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_status_str = ft_itoa(set_exit_status(1337, -1));
		if (!exit_status_str)
			return (free(var_name));
		value = exit_status_str;
	}
	else
		value = get_env_value(vars->env, var_name);
	if (value)
	{
		ft_strcpy(&result[vars->j], value);
		vars->j += ft_strlen(value);
	}
	if (!ft_strcmp(var_name, "?"))
		free(exit_status_str);
	free(var_name);
}

void	handle_variable_expansion(t_expand_vars *vars, char *result)
{
	int	dollar_count;

	dollar_count = count_dollars(vars->str, vars->i);
	if (dollar_count % 2 == 1)
	{
		vars->i += dollar_count - 1;
		if (vars->str[vars->i] == '$' && vars->str[vars->i + 1]
			&& !ft_isdigit(vars->str[vars->i + 1]))
			expand_and_copy_value(vars, result, NULL);
		else
		{
			if (ft_isdigit(vars->str[vars->i + 1]))
				vars->i = vars->i + 2;
			else
			{
				result[vars->j++] = '$';
				vars->i++;
			}
		}
	}
	else
		vars->i += dollar_count;
}

char	*expand_variables(char *str, t_env *env, t_token *tokens)
{
	t_expand_vars	vars;
	char			*result;
	size_t			expanded_size;

	init_expand_vars(&vars, str, env);
	expanded_size = expanded_length(str, env, &vars);
	result = malloc(expanded_size + 1);
	if (!result)
		return (NULL);
	while (str[vars.i])
	{
		quotes_state(str[vars.i], &vars.in_single, &vars.in_double);
		if (str[vars.i] == '$' && !vars.in_single && str[vars.i + 1])
		{
			handle_variable_expansion(&vars, result);
			tokens->var_in_quotes = vars.in_double;
			tokens->expanded_flag = 1;
		}
		else
			result[vars.j++] = str[vars.i++];
	}
	result[vars.j] = '\0';
	return (result);
}
