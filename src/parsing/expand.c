/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:31 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/23 15:12:04 by hmnasfa          ###   ########.fr       */
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

static int ft_numlen(int n)
{
	int	len;
	
	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	add_env_length(char *str, int *i, size_t *lenght, t_env *env)
{
	int		save_i;
	char	*var_name;
	char	*value;

	var_name = NULL;
	save_i = *i;
	*i = extract_var_name(str, *i, &var_name);
	if (var_name && *var_name)
	{
		if (ft_strcmp(var_name, "?") == 0)
			*lenght += ft_numlen(set_exit_status(1337, -1));
		else
		{
			value = get_env_value(env, var_name);
			if (value)
				*lenght += ft_strlen(value);
		}
	}
	else
	{
		*lenght += 1;
		*i = save_i + 1;
	}
	free(var_name);
}

size_t	expanded_length(char *str, t_env *env)
{
	size_t	lenght;
	int		i;

	i = 0;
	lenght = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			add_env_length(str, &i, &lenght, env);
		else
		{
			lenght++;
			i++;
		}
	}
	return (lenght);
}

static void	expand_and_copy_value(t_expand_vars *vars, char *result)
{
	char	*var_name;
	char	*value;
	char	*exit_status_str;

	var_name = NULL;
	vars->i = extract_var_name(vars->str, vars->i, &var_name);
	if (ft_strcmp(var_name, "?") == 0)
	{
		printf("----- %d\n", set_exit_status(1337, -1));
		exit_status_str = ft_itoa(set_exit_status(1337, -1));
		if (!exit_status_str)
		{
			free(var_name);
			return ;
		}
		value = exit_status_str;
	}
	else
		value = get_env_value(vars->env, var_name);
	if (value)
	{
		ft_strcpy(&result[vars->j], value);
		vars->j += ft_strlen(value);
	}
	if (ft_strcmp(var_name, "?") == 0)
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
			expand_and_copy_value(vars, result);
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

char	*expand_variables(char *str, t_env *env, int init_i, int init_j, t_token *tokens, int track)
{
	t_expand_vars	vars;
	char			*result;
	size_t			expanded_size;

	init_expand_vars(&vars, str, env, init_i, init_j);
	expanded_size = expanded_length(str, env);
	result = malloc(expanded_size + 1);
	printf("Allocated size = %zu\n", expanded_size);
	if (!result)
		return (NULL);
	while (str[vars.i])
	{
		quotes_state(str[vars.i], &vars.in_single, &vars.in_double);
		if (str[vars.i] == '$' && !vars.in_single && str[vars.i + 1])
		{
			handle_variable_expansion(&vars, result);
			if (track == 0)
				tokens->var_in_quotes = vars.in_double;
			tokens->expanded_flag = 1;
		}
		else
			result[vars.j++] = str[vars.i++];
	}
	printf("Final write index (j) = %d\n", vars.j);
	result[vars.j] = '\0';
	return (result);
}
