/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:30:59 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/07 20:27:36 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

size_t	expanded_herdoc_length(char *str, t_env *env)
{
	size_t	lenght;
	int		i;
	char	*var_name;

	i = 0;
	lenght = 0;
	var_name = NULL;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
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

char	*expand_herdoc_variables(char *str, t_env *env, int init_i, int init_j)
{
	t_expand_vars	vars;
	char			*result;

	init_expand_vars(&vars, str, env, init_i, init_j);
	result = malloc(expanded_herdoc_length(str, env) + 1);
	if (!result)
		return (NULL);
	while (str[vars.i]) 
	{
		if (str[vars.i] == '$' && !vars.in_single)
			handle_variable_expansion(&vars, result);
		else
			result[vars.j++] = str[vars.i++];
	}
	result[vars.j] = '\0';
	return (result);
}