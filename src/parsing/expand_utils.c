/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:46:36 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/12 11:42:25 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_expand_vars(t_expand_vars *vars, char *str, t_env *env)
{
	vars->str = str;
	vars->i = 0;
	vars->j = 0;
	vars->env = env;
	vars->in_double = 0;
	vars->in_single = 0;
}

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

int	ft_numlen(int n)
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

void	add_env_length(char *str, int *i, size_t *length, t_env *env)
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
			*length += ft_numlen(set_exit_status(1337, -1));
		else
		{
			value = get_env_value(env, var_name);
			if (value)
				*length += ft_strlen(value);
		}
	}
	else
	{
		*length += 1;
		*i = save_i + 1;
	}
	free(var_name);
}
