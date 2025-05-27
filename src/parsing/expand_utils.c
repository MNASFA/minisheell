/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:46:36 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/26 17:56:27 by hmnasfa          ###   ########.fr       */
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
